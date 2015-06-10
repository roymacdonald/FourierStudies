#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	sampleRate = 44100;
	bufferSize = maxBufferSize = getBinSizeFromFreq(getNoteFreq(NOTE_C0), sampleRate);
	cout << "maxBufferSize: " << maxBufferSize << endl;
	
	buffer = new float [bufferSize];
	memset(buffer, 0, bufferSize * sizeof(float));
	numNotes = (int)NOTE_B8 - NOTE_C0;
	bufferFreq = getNoteFreq(NOTE_C0);
	bufferFreqIndex = 0;
	
	bufferSize = 11;
	bufferFreq = sampleRate / (float)bufferSize;
	
	fillBuffer(buffer, bufferSize, bufferFreq, sampleRate);
}
//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::exit(){
	delete [] buffer;
}
//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(0);
	
	float xInc = ofGetWidth()/((float)bufferSize-1);
	
	ofMesh w, p, l;
	w.setMode(OF_PRIMITIVE_LINE_STRIP);
	p.setMode(OF_PRIMITIVE_POINTS);
	p.disableColors();
	l.setMode(OF_PRIMITIVE_LINES);
	for (int i =0; i < bufferSize; i++) {
		w.addVertex(ofVec3f(i*xInc, buffer[i]*200 + 200));
		w.addColor(ofFloatColor::white);
		p.addVertex(ofVec3f(i*xInc, buffer[i]*200 + 200));
		l.addVertex(ofVec3f(0, buffer[i]*200 + 200));
		l.addVertex(ofVec3f(ofGetWidth(), buffer[i]*200 + 200));
		l.addColor(ofFloatColor::gray);
		l.addColor(ofFloatColor::gray);
	}
	
	
	ofPushStyle();
	ofSetLineWidth(1);
	l.draw();
	w.draw();
	
	ofSetColor(ofColor::black);
	glPointSize(9);
	p.draw();
	ofSetColor(ofColor::white);
	glPointSize(7);
	p.draw();
	ofSetColor(ofColor::blue);
	glPointSize(5);
	p.draw();
	
	
	string msg = "bufferFreq: " + ofToString(bufferFreq) + "\n";
	msg += "bufferSize: " + ofToString(bufferSize) + "\n";
	msg += "bufferFreqIndex: " + ofToString(bufferFreqIndex) + "\n";
	ofPopStyle();
	ofDrawBitmapString(msg  , 100, ofGetHeight()-100);
}
//*/
//--------------------------------------------------------------
void ofApp::fillBuffer(float *b, int bSize, float freq, int sampRate, bool bSin){
	memset(b, 0, bSize * sizeof(float));
		if (bSin) {
			createSinWave(b, freq, sampRate, bSize);
		}else{
			createCosWave(b, freq, sampRate, bSize);
		}
}
//--------------------------------------------------------------
void ofApp::calcBandwidth(){
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	if(key == OF_KEY_UP){
		bufferFreqIndex ++;
		bufferFreqIndex %= numNotes;
		bufferFreq = getNoteFreq(Notes(bufferFreqIndex));
		bufferSize = getBinSizeFromFreq(bufferFreq, sampleRate);
		
		fillBuffer(buffer, bufferSize, bufferFreq,sampleRate);
	}else if(key == OF_KEY_DOWN){
		bufferFreqIndex --;
		if (bufferFreqIndex < 0) {
			bufferFreqIndex = numNotes-1;
		}
		bufferFreq = getNoteFreq(Notes(bufferFreqIndex));
		bufferSize = getBinSizeFromFreq(bufferFreq, sampleRate);
		fillBuffer(buffer, bufferSize, bufferFreq, sampleRate);
	}
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}



