#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	bufferSize = 1024;
	buffer = new float [bufferSize];
	freqs = new float [bufferSize];
	bw = new float [bufferSize];
#ifdef DO_DFT
	bDrawDft = true;
	bwD = new float [bufferSize];
#endif
	sampRate = 44100;
	memset(buffer, 0, bufferSize * sizeof(float));
	memset(freqs, 0, bufferSize * sizeof(float));
	memset(bw, 0, bufferSize * sizeof(float));
#ifdef DO_DFT
	if(bDoDft){
		memset(bwD, 0, bufferSize * sizeof(float));
	}
#endif
	bufferFreq = bufferSize/4;
	bDoDft = false;
	
	fft.set(bufferSize);
#ifdef DO_DFT
	if(bDoDft){
		dft.set(bufferSize);
	}
#endif
	binNum = bufferSize/4;
	
	bwMult = 1;
	minFreq =1;
	maxFreq = bufferSize/2;

	startBin = 0;
	endBin = fft.getNumBins();
	
	fillBuffer(buffer, bufferSize, bufferFreq);
	calcBandwidth();

	lastClick = ofGetElapsedTimeMillis();
	mesh.setMode(OF_PRIMITIVE_POINTS);
	
}
//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::exit(){
	delete [] buffer;
	delete [] freqs;
	delete [] bw;
#ifdef DO_DFT
	delete [] bwD;
#endif
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	float xInc = ofGetWidth()/(float)bufferSize;
		
	ofMesh b;
	b.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		b.addVertex(ofVec3f(i*xInc, (bw[i] *bwMult)));
		b.addVertex(ofVec3f(i*xInc, 0));
		b.addColor(ofFloatColor::blue);
		b.addColor(ofFloatColor::blue);
	}
#ifdef DO_DFT
	ofMesh bD;
	if(bDoDft){

	bD.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		bD.addVertex(ofVec3f(i*xInc, (bwD[i] *bwMult)));
		bD.addVertex(ofVec3f(i*xInc, 0));
		bD.addColor(ofFloatColor(ofFloatColor::yellow, 100));
		bD.addColor(ofFloatColor(ofFloatColor::yellow, 100));
	}
}
#endif

	ofPushStyle();
		
	
	ofSetLineWidth(1);
	b.draw();
#ifdef DO_DFT
	if (bDoDft) {
		if (bDrawDft) {
		//	ofEnableBlendMode(OF_BLENDMODE_ADD);
			bD.draw();
		//	ofEnableAlphaBlending();
		}
	}
#endif
	int ind = (int)floor(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, bufferSize));
	ofDrawBitmapStringHighlight("Freq: " + ofToString(freqs[ind]) + " val: " + ofToString(bw[ind]), ofGetMouseX(), ofGetMouseY()+10);
	

	string msg = "bufferFreq: " + ofToString(bufferFreq) + "\n";
	msg += "minFreq: " + ofToString(minFreq) + "\n";
	msg += "maxFreq: " + ofToString(maxFreq) + "\n";
	msg += "binNum: " + ofToString(binNum) ;
	
	if(r.width > 1 && r.height > 1){
		ofSetColor(40, 50);
		ofFill();
		ofDrawRectangle(r);
		ofSetColor(0, 50);
		ofNoFill();
		ofDrawRectangle(r);
	}
	ofPopStyle();
	ofDrawBitmapString(msg  , 100, ofGetHeight()-100);
	
	//cam.begin();
	//mesh.draw();
	//cam.end();
	
	
}
//--------------------------------------------------------------
void ofApp::fillBuffer(float *b, int bSize, float freq, bool bSin){
	memset(b, 0, bSize * sizeof(float));
	for (int i =0; i < bSize; i++) {
		float val = freq * TWO_PI * i/(float)bufferSize;
		if (bSin) {
			b[i] = -sin(val);
		}else{
			b[i] = cos(val);
		}
	}
}
//--------------------------------------------------------------
void ofApp::calcBandwidth(){
	memset(freqs, 0, bufferSize * sizeof(float));
	mesh.clear();
	float bMax = FLT_MIN;
	float xInc = ofGetWidth()/(float)fft.getNumBins();
	for (int j = 0; j < bufferSize; j++) {
		freqs[j] = ofMap(j, 0, bufferSize, minFreq, maxFreq);
		fillBuffer(buffer, bufferSize, freqs[j]);
		fft.forward(buffer, bufferSize);
#ifdef DO_DFT
		if (bDoDft) {
			dft.forward(buffer, bufferSize);
		}
#endif
		/*
		for (int i = startBin; i < endBin; i++) {
			//mesh.addVertex(ofVec3f((i-1)*xInc, fft.getSpectrum()[i-1], j*10));
			//mesh.addVertex(ofVec3f(i*xInc, fft.getSpectrum()[i], j*10));
			float alpha = fft.getSpectrum()[i]/(bufferSize*0.5f);
			mesh.addVertex(ofVec3f(ofMap(j, 0, bufferSize, 0, ofGetWidth()), ofMap(i, startBin, endBin, 0, ofGetHeight())));
			if (j == binNum) {
				mesh.addColor(ofFloatColor(1.0f, 1.0f,0.0f, alpha));
//				mesh.addColor(ofFloatColor::cyan);
			}else{
				mesh.addColor(ofFloatColor(1.0f,alpha));
//				mesh.addColor(ofFloatColor::yellow);
			}
		}
		//*/
#ifdef DO_DFT
		if (bDoDft) {
			bwD[j] = dft.getSpectrum()[binNum];
		}
#endif
		bw[j] = fft.getSpectrum()[binNum];
	}
	cout << "Thanks for the ride." << endl;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_RIGHT) {
		binNum++;
		binNum %= fft.getNumBins();
	//	calcBandwidth();
	}else 	if (key == OF_KEY_LEFT) {
		binNum--;
		binNum %= fft.getNumBins();
	//	calcBandwidth();
	}else if(key == OF_KEY_UP){
		bufferFreq++;
		fillBuffer(buffer, bufferSize, bufferFreq);
	}else if(key == OF_KEY_DOWN){
		bufferFreq--;
		fillBuffer(buffer, bufferSize, bufferFreq);
		
	}else if(key == 'b'){
		calcBandwidth();
	}else if(key == 'd'){
		bDoDft ^= true;
		if(bDoDft){
			dft.set(bufferSize);
		}
	}else if(key == ' '){
		bDrawDft ^=true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	r.width = x - r.x;
	r.height = y - r.y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
	r.x = x;
	r.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	unsigned long t = ofGetElapsedTimeMillis();
	if (t -lastClick < 300) {
		minFreq = 1;
		maxFreq =4000;
		calcBandwidth();
	}else{
		if (r.width > 1.0f && r.height > 1.0f) {
			r.width = x - r.x;
			r.height = y - r.y;
			
			float tempMinFreq = ofMap(r.x, 0, ofGetWidth(), minFreq, maxFreq);
			float tempMaxFreq = ofMap(r.getMaxX(), 0, ofGetWidth(), minFreq, maxFreq);
			int tempStartBin = ofMap(r.y, 0, ofGetHeight(), startBin, endBin);
			int tempEndBin = ofMap(r.getMaxY(), 0, ofGetHeight(), startBin, endBin);
			startBin = tempStartBin;
			endBin = tempEndBin;
			minFreq = tempMinFreq;
			maxFreq = tempMaxFreq;
			calcBandwidth();
		}else{
			bwMult = ofMap(y, 0, ofGetHeight(), 0, 1);
			
		}
	}
	lastClick = t;
}



