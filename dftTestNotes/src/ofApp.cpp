#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	bSetup = false;
	renderer.setup(NDFT);
	bufferSize = 1024;
	buffer = new float [bufferSize];
	sampRate = 44100;
	cargar();
	//NDFT.setup(NOTE_A1, NOTE_A7, bufferSize, sampRate);
	binNum = 0;
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
	fbo.begin();
	ofClear(0);
	fbo.end();
	/*
	float xInc = ofGetWidth()/(float)bufferSize;
	//float yInc = (ofGetHeight()-200)/((float)NDFT.getNumBins()/12);
	float yInc = 100;
	ofMesh l;
	l.setMode(OF_PRIMITIVE_LINES);
	cout << "NDFT.getNumBins(): " << NDFT.getNumBins()<< endl;
	for (int i =0; i < NDFT.getNumBins(); i+=12) {
		ofMesh s;
		//l.addVertex(ofVec3f(0, yInc*floor(i/12.0f) +100));
		//l.addVertex(ofVec3f(ofGetWidth(), yInc*floor(i/12.0f) +100));
		//l.addColor(ofFloatColor::white);
		//l.addColor(ofFloatColor::white);
		
		s.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int j =0; j < bufferSize; j++) {
//			s.addVertex(ofVec3f(j*xInc, NDFT.getSinLUT()[i*bufferSize+ j]*100  + yInc*floor(i/12.0f) +100));
			s.addVertex(ofVec3f(j*xInc, NDFT.getSinLUT()[i*bufferSize+ j]*100  +100));
			//ofFloatColor col;
			//col.setHsb((i%12)/12.0f, 1.0, 1.0);
			//s.addColor(col);
			s.addColor(ofFloatColor::red);
		}
		fbo.begin();
		s.draw();
		l.draw();
		
		fbo.end();
	}//*/
}
//--------------------------------------------------------------
void ofApp::cargar(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select audio file...");
    if (openFileResult.bSuccess){
        ofSoundFile sound;
        sound.loadSound(openFileResult.getPath());
        ofSoundBuffer buff;
        sound.readTo(buff);
        sampRate = sound.getSampleRate();
        audio = buff.getBuffer();
        NDFT.setup(NOTE_A1, NOTE_A7, bufferSize, sampRate);
		NDFT.processAll(audio, true);
    }
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
	
	renderer.draw(ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
	//ofSetColor(255);
	//fbo.draw(0,0);
	if (audio.size() >0) {
		float xInc = ofGetWidth() / (float)audio.size();
		ofMesh m;
		m.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int i =0; i< audio.size(); i++) {
			m.addVertex(ofVec3f(i*xInc, audio[i] * 20 +20 ));
			m.addColor(ofFloatColor::blue);
		}
		m.draw();
	}
	/*
	if (NDFT.isProcessed()) {
		float xInc = ofGetWidth() / (float)NDFT.procesadoSize;
		ofMesh m;
		m.setMode(OF_PRIMITIVE_LINE_STRIP);
		for (int i =0; i< NDFT.procesadoSize; i++) {
			m.addVertex(ofVec3f(i*xInc,NDFT.getProcessed()[i] * 100 +300 ));
			m.addColor(ofFloatColor::yellow);
		}
		m.draw();
	}//*/
	/*
	ofMesh w;
	w.setMode(OF_PRIMITIVE_LINE_STRIP);
	float xInc = ofGetWidth()/(float)bufferSize;
	for (int i =0; i < bufferSize; i++) {
		w.addVertex(ofVec3f(i*xInc, buffer[i]*100 + 100));
		w.addColor(ofFloatColor::white);
	}
	w.draw();
	//*/
	/*
	ofPushStyle();
	ofSetLineWidth(3);
	if (NDFT.isSetup()) {
		ofMesh s;
		s.setMode(OF_PRIMITIVE_LINE_STRIP);
		xInc = ofGetWidth()/(float)bufferSize;
		for (int i =0; i < bufferSize; i++) {
			//		s.addVertex(ofVec3f(i*xInc, NDFT.getSinLUT()[binNum*bufferSize+ i]*100 + 300));
			s.addVertex(ofVec3f(i*xInc, ndft::createCosWave(NDFT.noteToFreq(NOTE_A2), sampRate, i)*100 + 200));
			s.addColor(ofFloatColor::cyan);
		}
		s.draw();
		
		ofMesh c;
		c.setMode(OF_PRIMITIVE_LINE_STRIP);
		xInc = ofGetWidth()/(float)bufferSize;
		for (int i =0; i < bufferSize; i++) {
			//		c.addVertex(ofVec3f(i*xInc, NDFT.getCosLUT()[binNum*bufferSize+ i]*100 + 500));
			c.addVertex(ofVec3f(i*xInc, ndft::createCosWave(NDFT.noteToFreq(NOTE_A3), sampRate, i)*100 + 300));
			c.addColor(ofFloatColor::pink);
		}
		c.draw();
		
		ofMesh c1;
		c1.setMode(OF_PRIMITIVE_LINE_STRIP);
		xInc = ofGetWidth()/(float)bufferSize;
		for (int i =0; i < bufferSize; i++) {
			//		c.addVertex(ofVec3f(i*xInc, NDFT.getCosLUT()[binNum*bufferSize+ i]*100 + 500));
			c1.addVertex(ofVec3f(i*xInc, ndft::createCosWave(NDFT.noteToFreq(NOTE_A4), sampRate, i)*100 + 400));
			c1.addColor(ofFloatColor::pink);
		}
		c1.draw();
		
	}
	/*
	if (NDFT.isProcessed()) {
		ofMesh m;
		m.setMode(OF_PRIMITIVE_LINES);
		float yInc = (ofGetHeight()-200)/(float)NDFT.getNumBins();
		for (int i = 0; i<NDFT.getNumBins(); i++) {
			m.addVertex(ofVec3f(NDFT.getSpectrum()[i] * 10, i*yInc + 200));
			m.addVertex(ofVec3f(0, i*yInc + 200));
			m.addColor(ofFloatColor::yellow);
			m.addColor(ofFloatColor::yellow);
		}
		m.draw();
	}
	
	ofPopStyle();
	 //*/
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_RIGHT) {
		binNum++;
		binNum %= NDFT.getNumBins();
	}else 	if (key == OF_KEY_LEFT) {
		binNum--;
		binNum %= NDFT.getNumBins();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	memset(buffer, 0, bufferSize);
	for (int i =0; i < bufferSize; i++) {
		buffer[i] = ndft::createCosWave(ofMap(x, 0, ofGetWidth(), 110, 7000), sampRate, i);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	NDFT.process(buffer, bufferSize);
}



