#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	bufferSize = 4096;
	buffer = new float [bufferSize];
	lut = new float [bufferSize];
	lutS = new float [bufferSize];
	bw = new float [bufferSize];
	bwS = new float [bufferSize];
	bwH = new float [bufferSize];
	freqs = new float [bufferSize];
	sampRate = 44100;
	memset(buffer, 0, bufferSize * sizeof(float));
	memset(lut, 0, bufferSize * sizeof(float));
	memset(lutS, 0, bufferSize * sizeof(float));
	memset(bw, 0, bufferSize * sizeof(float));
	memset(bwS, 0, bufferSize * sizeof(float));
	memset(bwH, 0, bufferSize * sizeof(float));
	memset(freqs, 0, bufferSize * sizeof(float));
	bufferFreq = bufferSize/4;
	lutFreq = 10;//bufferSize/4;
	lutMinMargin = 0;
	lutMaxMargin = bufferSize;
	sum = 0;
	bwMult = 1;
	minFreq =1;
	maxFreq = 4000;
	bWindow = true;
	
	fillBuffer(buffer, bufferSize, bufferFreq);
	window.apply(buffer, bufferSize);
	fillBuffer(lut, bufferSize, lutFreq);
	fillBuffer(lutS, bufferSize, lutFreq, true);
	calcBandwidth();
	window.setup(bufferSize);
	lastClick = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::exit(){
	delete [] buffer;
	delete [] lut;
	delete [] lutS;
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
	
	ofMesh l;
	l.setMode(OF_PRIMITIVE_LINE_STRIP);
	int i0 = lutMinMargin;
	int i1 = lutMaxMargin;
	float lutXInc = ofGetWidth()/(float)(lutMaxMargin - lutMinMargin);
	if (ofGetKeyPressed(' ')) {
		i0 = 0; i1 = bufferSize;
		lutXInc = xInc;
	}
	for (int i =i0; i < i1; i++) {
		l.addVertex(ofVec3f((i - i0)*lutXInc, lut[i]*100 + 300));
		l.addColor(ofFloatColor::cyan);
	}
	
	ofMesh ls;
	ls.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int i =i0; i < i1; i++) {
		ls.addVertex(ofVec3f((i - i0)*lutXInc, lutS[i]*100 + 300));
		ls.addColor(ofFloatColor::magenta);
	}
	if (ofGetWidth()/(float)(lutMaxMargin - lutMinMargin) > 15) {
		ofMesh g;
		g.setMode(OF_PRIMITIVE_LINES);
		for (int i =i0; i < i1; i++) {
			g.addVertex(ofVec3f((i - i0)*lutXInc, 200));
			g.addVertex(ofVec3f((i - i0)*lutXInc, 400));
			g.addColor(ofFloatColor(1.0f, 0.3f));
			g.addColor(ofFloatColor(1.0f, 0.3f));
		}
		ofPushMatrix();
		ofSetLineWidth(1);
		g.draw();
		ofPopMatrix();
	}

	
	ofMesh m;
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int i =0; i < bufferSize; i++) {
		m.addVertex(ofVec3f(i*xInc, lut[i] * buffer[i]*100 + 500));
		m.addColor(ofFloatColor::yellow);
	}
	
	ofMesh b;
	b.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
	//	b.addVertex(ofVec3f(i*xInc, ((bw[i] / (bufferSize/2))*100 + 550)));
//		b.addVertex(ofVec3f(i*xInc, (lut[i] *100 + 500)));
		b.addVertex(ofVec3f(i*xInc, (bw[i] *bwMult +100)));
		b.addVertex(ofVec3f(i*xInc, 100));
		b.addColor(ofFloatColor::red);
		b.addColor(ofFloatColor::red);
	}

	ofMesh bS;
	bS.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		//	b.addVertex(ofVec3f(i*xInc, ((bw[i] / (bufferSize/2))*100 + 550)));
		//		b.addVertex(ofVec3f(i*xInc, (lut[i] *100 + 500)));
		bS.addVertex(ofVec3f(i*xInc, (bwS[i] *bwMult +200)));
		bS.addVertex(ofVec3f(i*xInc, 200));
		bS.addColor(ofFloatColor::blue);
		bS.addColor(ofFloatColor::blue);
	}

	ofMesh bH;
	bH.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		//	b.addVertex(ofVec3f(i*xInc, ((bw[i] / (bufferSize/2))*100 + 550)));
		//		b.addVertex(ofVec3f(i*xInc, (lut[i] *100 + 500)));
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
	m.draw();
	l.draw();
	ls.draw();
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
	
	string msg = "Integral " + ofToString(sum) + "\n";
	msg += "bufferFreq: " + ofToString(bufferFreq) + "\n";
	msg += "lutFreq: " + ofToString(lutFreq) + "\n";
	msg += "minFreq: " + ofToString(minFreq) + "\n";
	msg += "maxFreq: " + ofToString(maxFreq) + "\n";
	msg += "lutMinMargin: " + ofToString(lutMinMargin) + "\n";
	msg += "lutMaxMargin: " + ofToString(lutMaxMargin);
	if(r.width > 1 && r.height > 1){
		ofSetColor(40, 50);
		ofFill();
		ofDrawRectangle(r);
		ofSetColor(0, 50);
		ofNoFill();
		ofDrawRectangle(r);
	//	string m = "min: " + ofToString(ofMap(r.x, 0, ofGetWidth(), lutFreq -bwWidth, lutFreq +bwWidth )) + "\n";
	//	m += "max: " + ofToString(ofMap(r.getMaxX(), 0, ofGetWidth(), lutFreq -bwWidth, lutFreq +bwWidth ));
	//	ofDrawBitmapStringHighlight(m, r.x, r.getMaxY()+10);
	}
	ofPopStyle();
	ofDrawBitmapString(msg  , 100, ofGetHeight()-100);
}
//--------------------------------------------------------------
void ofApp::sumAll(){
	sum = 0;
	for (int i = 0; i < bufferSize; i++) {
		sum += lut[i] * buffer[i];
	}
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
	sumAll();
}
//--------------------------------------------------------------
float ofApp::cosineIntegral(float x, float a1, float a2){

	return sin((a2-a1)*x)/(2 * (a2-a1) ) +  sin((a2-a1)*x)/(2 * (a2-a1));
	
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
		if (bWindow) {
			window.apply(buffer, bufferSize);
		}
		for (int i = 0; i < bufferSize; i++) {
			float r = lut[i] * buffer[i];
			float im= lutS[i] * buffer[i];
			bw[j] += r;
			bwS[j] += im;
		}
		bwH[j] = sqrt(bw[j] * bw[j] + bwS[j] * bwS[j]);
		if (bwH[j] > bMax ) {
			bMax = bwH[j];
			bN = j;
		}
		if(bwH[j] > 2000){
			cout << "Bin: " << j << " freq: " << freqs[j] << " val: " << bwH[j] << endl;
		}
	//	bw[j] /= (float)bufferSize/2.0f;
	//	cout << bw[j] << endl;
	}
	cout << "Max. bin["<<bN<<"] val: " << bMax << endl;
	cout << "Thanks for the ride." << endl;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_RIGHT) {
		lutFreq++;
		//lutFreq %= NDFT.getNumBins();
		fillBuffer(lut, bufferSize, lutFreq);
		fillBuffer(lutS, bufferSize, lutFreq, true);
	}else 	if (key == OF_KEY_LEFT) {
		lutFreq--;
		//lutFreq %= NDFT.getNumBins();
		fillBuffer(lut, bufferSize, lutFreq);
		fillBuffer(lutS, bufferSize, lutFreq, true);
	}else if(key == OF_KEY_UP){
		bufferFreq++;
		fillBuffer(buffer, bufferSize, bufferFreq);
		if (bWindow) {
			window.apply(buffer, bufferSize);
		}
	}else if(key == OF_KEY_DOWN){
		bufferFreq--;
		fillBuffer(buffer, bufferSize, bufferFreq);
		if (bWindow) {
			window.apply(buffer, bufferSize);
		}
	}else if(key == 'b'){
		calcBandwidth();
	}else if(key == 'w'){
		bWindow ^= true;
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
	unsigned long t = ofGetElapsedTimeMillis();
	if (t -lastClick < 300) {
		minFreq = 1;
		maxFreq =4000;
		calcBandwidth();
		lutMinMargin = 0;
		lutMaxMargin = bufferSize;
	}else{
		r.x = x;
		r.y = y;
	}
	lastClick = t;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (r.width > 1.0f && r.height > 1.0f) {
		r.width = x - r.x;
		r.height = y - r.y;
		if (y < 200) {
			float tempMinFreq = ofMap(r.x, 0, ofGetWidth(), minFreq, maxFreq);
			float tempMaxFreq = ofMap(r.getMaxX(), 0, ofGetWidth(), minFreq, maxFreq);
			minFreq = tempMinFreq;
			maxFreq = tempMaxFreq;
			calcBandwidth();
		}else{
			lutMinMargin = (int)floor(ofMap(r.x, 0, ofGetWidth(), 0, bufferSize));
			lutMaxMargin = (int)floor(ofMap(r.getMaxX(), 0, ofGetWidth(), 0, bufferSize));
		}
	}else{
		bwMult = ofMap(y, 0, ofGetHeight(), 0, 1);
		
	}
}



