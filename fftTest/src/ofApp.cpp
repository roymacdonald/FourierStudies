#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	fftsize = 256;
	fft.set(fftsize);
	samples = new float [fftsize];
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i< fftsize ; i++) {
		samples[i] = sinf(PI * i * mouseX / (float)ofGetWidth()) ;
	}
	fft.forward(samples, fftsize);
}
//--------------------------------------------------------------
void ofApp::exit(){
	delete [] samples;
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofMesh m;
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	float xInc = ofGetWidth()/(float)fft.getNumBins();
	for (int i = 0; i< fft.getNumBins() ; i++) {
		m.addVertex(ofVec3f(i*xInc, fft.getSpectrum()[i]));
		m.addColor(ofFloatColor::white);
	}
	m.draw();
	
	ofMesh mm;
	mm.setMode(OF_PRIMITIVE_LINE_STRIP);
	xInc = ofGetWidth()/(float)fftsize;
	for (int i =0; i < fftsize; i++) {
		mm.addVertex(ofVec3f(i*xInc, samples[i]*100));
		mm.addColor(ofFloatColor::yellow);
	}
	
	ofPushMatrix();
	ofTranslate(0, 400);
	mm.draw();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		for (int i =0; i < fft.getNumBins(); i++) {
			cout << "fft [" << i << "]: "<< fft.getSpectrum()[i] << endl;
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}