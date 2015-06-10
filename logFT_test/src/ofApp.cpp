#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	bSetup = false;
	renderer.setup(lDFT);
	
	waveRect.set(0,0,ofGetWidth(), 40);
	binRect.set(0, waveRect.getMaxY(), ofGetWidth(), 40);
	specRect.set(0, binRect.getMaxY(), ofGetWidth(), ofGetHeight()-waveRect.getMaxY());
	
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
	fbo.begin();
	ofClear(0);
	fbo.end();
	cargar();
	
	renderer.multData.addListener(this, &ofApp::guiChangedF);
	renderer.bandsOpacity.addListener(this, &ofApp::guiChangedI);
	
	bDrawGui = true;
	selectedBin=0;
}

//--------------------------------------------------------------
void ofApp::guiChangedF(float & f){
	render();
}
//--------------------------------------------------------------
void ofApp::guiChangedI(int & i){
	render();
}
//--------------------------------------------------------------
void ofApp::cargar(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select audio file...");
    if (openFileResult.bSuccess){
        ofSoundFile sound;
        sound.loadSound(openFileResult.getPath());
        ofSoundBuffer buff;
        sound.readTo(buff);
        int sampRate = sound.getSampleRate();
        audio = buff.getBuffer();
		cout << "Audio size: " << audio.size() << endl;
		if (audio.size() >0) {
			lDFT.setup(NOTE_A1, NOTE_A7, sampRate);
			lDFT.process(audio.data(), audio.size());
			render();
		}
    }
}
//--------------------------------------------------------------
void ofApp::render(){
	float xInc = ofGetWidth() / (float)audio.size();
	ofMesh m;
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	float yMult = waveRect.height/2;
	float yOffset = waveRect.height/2 + waveRect.x;
	for (int i =0; i< audio.size(); i++) {
		m.addVertex(ofVec3f(i*xInc, audio[i] * yMult +yOffset ));
		m.addColor(ofFloatColor::blue);
	}
	fbo.begin();
	m.draw();
	renderer.draw(specRect);
	fbo.end();
	renderBin();
}
//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::exit(){
}
//--------------------------------------------------------------
void ofApp::renderBin(){
	cout << "renderBin. selected bin: " << selectedBin << endl;
	float xInc = binRect.width/lDFT.getSpectrum()[selectedBin].size();
	ofMesh b;
	b.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int i =0 ; i < lDFT.getSpectrum()[selectedBin].size(); i++) {
		b.addVertex(ofVec3f(i*xInc, binRect.getMaxY() - binRect.height * lDFT.getSpectrum()[selectedBin][i]/lDFT.getMaximum() ));
		b.addColor(ofFloatColor::yellow);
	}
	
	ofPushStyle();
	fbo.begin();
	ofSetColor(0);
	ofFill();
	ofDrawRectangle(binRect);
	ofSetColor(255);
	ofSetLineWidth(2);
	b.draw();
	fbo.end();
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	
	ofSetColor(255);
	fbo.draw(0,0);
	if (bDrawGui) {
		renderer.drawGui();
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'g') {
		bDrawGui ^=true;
	}else if(key == OF_KEY_DOWN){
		selectedBin++;
		selectedBin%=lDFT.getNumBins();
		renderBin();
	}else if(key == OF_KEY_UP){
		selectedBin-=1;
		if (selectedBin<0) {selectedBin = 0;};
		renderBin();
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



