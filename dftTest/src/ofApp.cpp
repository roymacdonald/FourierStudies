#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	timeSize = 64;
	specLength = timeSize / 2.0f + 1;
	buildTrigTables();
	f = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::exit(){
	delete [] sinlookup;
	delete [] coslookup;
}
//--------------------------------------------------------------
void ofApp::draw(){
	
	int N = timeSize;
	ofMesh mR, mI, mS, mM, mBw;
	mR.setMode(OF_PRIMITIVE_LINE_STRIP);
	mI.setMode(OF_PRIMITIVE_LINE_STRIP);
	mS.setMode(OF_PRIMITIVE_LINE_STRIP);
	mM.setMode(OF_PRIMITIVE_LINE_STRIP);
	mBw.setMode(OF_PRIMITIVE_LINES);
	float xInc = ofGetWidth()/(float)N;
	float sum = 0;
	for (int t = 0; t < N; t++)
	{
		float co = coslookup[t * f];
		float si = sinlookup[t * f];
		float sig = cos(t*TWO_PI / (float)N);
		mR.addVertex(ofVec3f(t * xInc,  co*200 + ofGetHeight()/2));
		mI.addVertex(ofVec3f(t * xInc, -si*200 + ofGetHeight()/2));
		mS.addVertex(ofVec3f(t * xInc, sig*200 + ofGetHeight()/2));
		mM.addVertex(ofVec3f(t * xInc,  co * sig *200 + ofGetHeight()/2));
		mR.addColor(ofFloatColor::yellow);
		mI.addColor(ofFloatColor::cyan);
		mS.addColor(ofFloatColor::magenta);
		mM.addColor(ofFloatColor::white);
		
		sum += co * sig;
		
	}
	for (int i =0; i < N/2; i++) {
		float sum2=0;
		for (int t = 0; t < N; t++){
			float co = coslookup[t * i];
			float sig = cos(t*TWO_PI / (float)N);
			sum2 += co * sig;
		}
		
		xInc = 2*ofGetWidth()/N;
		
		mBw.addVertex(ofVec3f(xInc*i, (ofGetHeight()/2) - (100000*sum2)));
		mBw.addVertex(ofVec3f(xInc*i, ofGetHeight()/2));
		mBw.addColor(ofFloatColor::blue);
		mBw.addColor(ofFloatColor::blue);
	}
	
	ofBackground(0);

	
	ofSetLineWidth(1);
	mR.draw();
	mI.draw();
	mS.draw();
	ofSetLineWidth(2);
	mM.draw();
	mBw.draw();
	lookupMesh.draw();
	ofDrawBitmapStringHighlight("Sum 0 - 1: " + ofToString(sum), 20, 20);
	

}
//--------------------------------------------------------------
void ofApp::buildTrigTables()
{
	lookupMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    int N = specLength * timeSize;
	float xInc = ofGetWidth()/ (float)N;
    sinlookup = new float[N];
    coslookup = new float[N];
    for (int i = 0; i < N; i++){
		sinlookup[i] = (float) sin(i * TWO_PI / timeSize);
		coslookup[i] = (float) cos(i * TWO_PI / timeSize);
		lookupMesh.addVertex(ofVec3f(xInc*i, coslookup[i]*10 + 11));
		lookupMesh.addColor(ofFloatColor::white);
	//	cout << "coslookup["<<i<<"]: "<<coslookup[i] << " ,  " << sinlookup[i] << endl;
    }
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key== OF_KEY_LEFT) {
		f --;
		if (f < 0) f = 0;
	}else if (key == OF_KEY_RIGHT){
		f++;
		if (f > timeSize / 2) f = timeSize / 2;
	}
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