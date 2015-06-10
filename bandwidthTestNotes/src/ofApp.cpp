#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	bufferSize = 4096;
	minNote = NOTE_A0;
	maxNote = NOTE_B8;
	numNotes = (int)maxNote - minNote;
	cout << "numNotes: " << numNotes << endl;
	buffer = new float [bufferSize];
	lut = new float [bufferSize];
	lutS = new float [bufferSize];
	bw = new float [numNotes];
	bwS = new float [numNotes];
	bwH = new float [numNotes];
	freqs = new float [numNotes];
	
	sampRate = 44100;
	memset(buffer, 0, bufferSize * sizeof(float));
	memset(lut, 0, bufferSize * sizeof(float));
	memset(lutS, 0, bufferSize * sizeof(float));
	
	memset(bw, 0, numNotes * sizeof(float));
	memset(bwS, 0, numNotes * sizeof(float));
	memset(bwH, 0, numNotes * sizeof(float));
	memset(freqs, 0, numNotes * sizeof(float));
	
	bufferFreq = getNoteFreq(NOTE_A5);
	lutFreq = getNoteFreq(NOTE_A4);
	lutMinMargin = 0;
	lutMaxMargin = bufferSize;
	sum = 0;
	bwMult = 1;
	bWindow = true;
	window.setup(bufferSize);
	lutFreqIndex = 0;
	
	bufferFreqIndex = 0;
	
	fillBuffer(buffer, bufferSize, bufferFreq);
	window.apply(buffer, bufferSize);
	fillBuffer(lut, bufferSize, lutFreq);
	fillBuffer(lutS, bufferSize, lutFreq, true);
	calcBandwidth();
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
	delete [] freqs;
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
		for (int i =0; i < bufferSize; i++) {
			l.addVertex(ofVec3f(i *xInc, lut[i]*100 + 300));
			l.addColor(ofFloatColor::cyan);
		}
		
		ofMesh ls;
		ls.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int i =0; i < bufferSize; i++) {
			ls.addVertex(ofVec3f(i *xInc, lutS[i]*100 + 300));
			ls.addColor(ofFloatColor::magenta);
		}
		
		float xNoteInc = ofGetWidth()/(float)numNotes;
		
		ofMesh b;
		b.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int i =0; i < numNotes; i++) {
			b.addVertex(ofVec3f(i*xNoteInc, (bw[i] *bwMult +100)));
//			b.addVertex(ofVec3f(i*xNoteInc, 100));
			b.addColor(ofFloatColor::red);
//			b.addColor(ofFloatColor::red);
		}
		
		ofMesh bS;
		bS.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int i =0; i < numNotes; i++) {
			bS.addVertex(ofVec3f(i*xNoteInc, (bwS[i] *bwMult +200)));
//			bS.addVertex(ofVec3f(i*xNoteInc, 200));
//			bS.addColor(ofFloatColor::blue);
			bS.addColor(ofFloatColor::blue);
		}
		
		ofMesh bH;
		bH.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int i =0; i < numNotes; i++) {
			bH.addVertex(ofVec3f(i*xNoteInc, (bwH[i] *bwMult +400)));
//			bH.addVertex(ofVec3f(i*xNoteInc, 400));
			bH.addColor(ofFloatColor::cyan);
//			bH.addColor(ofFloatColor::darkCyan);
		}
		
		ofPushStyle();
		
		ofSetColor(255);
		ofSetLineWidth(1);
		ofDrawLine(0, 100, ofGetWidth(), 100);
		ofDrawLine(0, 300, ofGetWidth(), 300);
		ofDrawLine(0, 500, ofGetWidth(), 500);
		
		
		
		ofSetLineWidth(1);
		w.draw();
		//	m.draw();
		l.draw();
		ls.draw();
		ofSetLineWidth(3);
		b.draw();
		bS.draw();
		bH.draw();
		
		
		
	//	int ind = (int)floor(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, bufferSize));
	//	ofDrawBitmapStringHighlight("Freq: " + ofToString(freqs[ind]) + " val: " + ofToString(bwH[ind]), ofGetMouseX(), ofGetMouseY()+10);
		
		string msg = "bufferFreq: " + ofToString(bufferFreq) + "\n";
		msg += "lutFreq: " + ofToString(lutFreq) + "\n";
		msg += "minFreq: " + ofToString(getNoteFreq(minNote)) + "\n";
		msg += "maxFreq: " + ofToString(getNoteFreq(maxNote));
 		ofPopStyle();
		ofDrawBitmapString(msg  , 100, ofGetHeight()-100);
	}
	 //*/
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
//*/
}
//--------------------------------------------------------------
void ofApp::calcBandwidth(){
	
	memset(bw, 0, numNotes * sizeof(float));
	memset(bwS, 0, numNotes * sizeof(float));
	memset(bwH, 0, numNotes * sizeof(float));
	memset(freqs, 0, numNotes * sizeof(float));
	float bMax = FLT_MIN;
	int bN = 0;
	for (int j = 0; j < numNotes; j++) {
		freqs[j] = getNoteFreq(Notes(minNote + j));
		
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
		bwMult = (ofGetHeight()*0.2f)/bMax;
		//	bw[j] /= (float)bufferSize/2.0f;
		//	cout << bw[j] << endl;
	}
	cout << "Max. bin["<<bN<<"] val: " << bMax << endl;
	cout << "Thanks for the ride." << endl;
	//*/
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	if (key == OF_KEY_RIGHT) {
		lutFreqIndex++;
		lutFreqIndex %= numNotes;
		lutFreq = freqs[lutFreqIndex];
		
		fillBuffer(lut, bufferSize, lutFreq);
		fillBuffer(lutS, bufferSize, lutFreq, true);
		calcBandwidth();
	}else 	if (key == OF_KEY_LEFT) {
		lutFreqIndex--;
		if (lutFreqIndex < 0) {
			lutFreqIndex = numNotes-1;
		}
		lutFreq = freqs[lutFreqIndex];
		
		fillBuffer(lut, bufferSize, lutFreq);
		fillBuffer(lutS, bufferSize, lutFreq, true);
		calcBandwidth();
	}else if(key == OF_KEY_UP){
		bufferFreqIndex ++;
		bufferFreqIndex %= numNotes;
		bufferFreq = freqs[bufferFreqIndex];
		fillBuffer(buffer, bufferSize, bufferFreq);
		if (bWindow) {
			window.apply(buffer, bufferSize);
		}
	}else if(key == OF_KEY_DOWN){
		bufferFreqIndex --;
		if (bufferFreqIndex < 0) {
			bufferFreqIndex = numNotes-1;
		}
		bufferFreq = freqs[bufferFreqIndex];
		fillBuffer(buffer, bufferSize, bufferFreq);
		if (bWindow) {
			window.apply(buffer, bufferSize);
		}
	}else if(key == 'b'){
		calcBandwidth();
	}else if(key == 'w'){
		bWindow ^= true;
	}
	//*/
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}



