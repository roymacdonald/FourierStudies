#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	bufferSize = 4096;
	buffer = new float [bufferSize];
	bw = new float [bufferSize];
	bwS = new float [bufferSize];
	bwH = new float [bufferSize];
	freqs = new float [bufferSize];
	sampRate = 44100;
	memset(buffer, 0, bufferSize * sizeof(float));
	memset(bw, 0, bufferSize * sizeof(float));
	memset(bwS, 0, bufferSize * sizeof(float));
	memset(bwH, 0, bufferSize * sizeof(float));
	memset(freqs, 0, bufferSize * sizeof(float));
	bufferFreq = bufferSize/4;

	fft.set(bufferSize);
	
	binNum = 1000;
	
	bwMult = 1;
	minFreq =1;
	maxFreq = bufferSize/2;
	
	fillBuffer(buffer, bufferSize, bufferFreq);
	calcBandwidth();
	lastClick = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::exit(){
	delete [] buffer;
	delete [] bw;
	delete [] bwS;
	delete [] bwH;
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	float xInc = ofGetWidth()/(float)bufferSize;
	
	ofMesh w;
	w.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int i =0; i < bufferSize; i++) {
		w.addVertex(ofVec3f(i*xInc, buffer[i]*100 + 100));
		w.addColor(ofFloatColor::white);
	}
	
	ofMesh b;
	b.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		b.addVertex(ofVec3f(i*xInc, (bw[i] *bwMult +100)));
		b.addVertex(ofVec3f(i*xInc, 100));
		b.addColor(ofFloatColor::red);
		b.addColor(ofFloatColor::red);
	}
	
	ofMesh bS;
	bS.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		bS.addVertex(ofVec3f(i*xInc, (bwS[i] *bwMult +200)));
		bS.addVertex(ofVec3f(i*xInc, 200));
		bS.addColor(ofFloatColor::blue);
		bS.addColor(ofFloatColor::blue);
	}
	
	ofMesh bH;
	bH.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		bH.addVertex(ofVec3f(i*xInc, (bwH[i] *bwMult +400)));
		bH.addVertex(ofVec3f(i*xInc, 400));
		bH.addColor(ofFloatColor::darkCyan);
		bH.addColor(ofFloatColor::darkCyan);
	}
	
	ofPushStyle();
	
	ofSetColor(255);
	ofSetLineWidth(1);
	ofDrawLine(0, 100, ofGetWidth(), 100);
	ofDrawLine(0, 300, ofGetWidth(), 300);
	ofDrawLine(0, 500, ofGetWidth(), 500);
	
	
	
	ofSetLineWidth(3);
	w.draw();
	ofSetLineWidth(1);
	b.draw();
	bS.draw();
	bH.draw();
	
	
	float half = ofMap(bufferSize/2, minFreq, maxFreq, 0, ofGetWidth());
	
	if (half>0 && half < ofGetWidth()) {
		ofSetColor(255);
		ofDrawLine(half, 0, half, 200);
	}
	
	int ind = (int)floor(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, bufferSize));
	ofDrawBitmapStringHighlight("Freq: " + ofToString(freqs[ind]) + " val: " + ofToString(bwH[ind]), ofGetMouseX(), ofGetMouseY()+10);
	

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
	memset(bw, 0, bufferSize * sizeof(float));
	memset(bwS, 0, bufferSize * sizeof(float));
	memset(bwH, 0, bufferSize * sizeof(float));
	memset(freqs, 0, bufferSize * sizeof(float));
	float bMax = FLT_MIN;
	int bN = 0;
	for (int j = 0; j < bufferSize; j++) {
		freqs[j] = ofMap(j, 0, bufferSize, minFreq, maxFreq);
		fillBuffer(buffer, bufferSize, freqs[j]);
		fft.forward(buffer, bufferSize);
		bw[j] = fft.getReal(binNum);
		bwS[j] = fft.getImag(binNum);
		bwH[j] = fft.getSpectrum()[binNum];
		if (bN < bwH[j]) {
			bN = bwH[j];
		}
	}
	cout << "max val: " << bN << endl;
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
			minFreq = tempMinFreq;
			maxFreq = tempMaxFreq;
			calcBandwidth();
		}else{
			bwMult = ofMap(y, 0, ofGetHeight(), 0, 1);
			
		}
	}
	lastClick = t;
}



