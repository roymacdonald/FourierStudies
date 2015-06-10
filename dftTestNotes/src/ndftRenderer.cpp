//
//  ndftRenderer.cpp
//  emptyExample
//
//  Created by Roy Macdonald on 4/30/15.
//
//

#include "ndftRenderer.h"
//
//  ndftRenderer.cpp
//  emptyExample
//
//  Created by Roy Macdonald on 4/25/15.
//
//

#include "ndftRenderer.h"


//--------------------------------------------------------------
void ndftRenderer::draw(ofRectangle r, bool bDrawBands){
	
	if(ndftPtr->isProcessed()){
		ofPixels pix;
		pix.allocate(ndftPtr->getCuantos(), ndftPtr->getNumBins(), 1);
		for(int i=0; i<ndftPtr->getCuantos(); i++){
			for(int j =0; j<ndftPtr->getNumBins();j++){
				pix.getData()[j *ndftPtr->getCuantos() + i] = (ndftPtr->getProcessed()[j + i* ndftPtr->getNumBins()]/ndftPtr->getMaximo())*255*multData;
			}
		}
		ofTexture tex;
		tex.allocate(pix, true);
		ofSetColor(255);
		if (bDrawBands) {
			drawBands(r);
		}
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		tex.draw(r.x, r.y, r.width, r.height);
		ofDisableBlendMode();
	}
}
//--------------------------------------------------------------
void ndftRenderer::drawVector(ofRectangle r, bool bDrawBands){
	if(ndftPtr->isProcessed()){
		ofMesh m;
		m.setMode(OF_PRIMITIVE_POINTS);
		float xInc = r.width/ndftPtr->getCuantos();
		float yInc = r.height/ndftPtr->getNumBins();
		for(int j =0; j<ndftPtr->getNumBins();j++){
			for(int i=0; i<ndftPtr->getCuantos(); i++){
				m.addVertex(ofVec3f(i * xInc, j * yInc));
				m.addColor(ofFloatColor(1.0, (ndftPtr->getProcessed()[j + i* ndftPtr->getNumBins()]/ndftPtr->getMaximo())*multData));
			}
		}
		if (bDrawBands) {
			drawBands(r);
		}
		ofPushMatrix();
		ofTranslate(r.x, r.y);
		ofEnableAlphaBlending();
		m.draw();
		ofPopMatrix();
	}
	
}
//--------------------------------------------------------------
void ndftRenderer::drawBands(ofRectangle r){
	int noteOffSet = (ndftPtr->getStartNote()%12);
	bool  sharps [] = {false,true,false,false,true,false,true,false,false,true,false,true};
	string notes []= {"A","","B","C","","D","","E","F","","G",""};
	
	float inc = r.height/(ndftPtr->getEndNote() - ndftPtr->getStartNote());
	ofPushStyle();
	int  o=noteOffSet;
	for (float i=0; i<r.height; i+=inc,o++){
		if(sharps[o%12]){
			ofSetColor(0, bandsOpacity);
		}
		else{
			ofSetColor(255, bandsOpacity);
		}
		ofFill();
		ofDrawRectangle(r.x,i-(inc/2)+r.y,r.width,inc);
		ofSetColor(0, bandsOpacity);
		ofNoFill();
		ofDrawRectangle(r.x,i-(inc/2)+r.y,r.width,inc);
		ofSetColor(255, bandsOpacity);
		ofDrawBitmapString(notes[o%12],r.width/2,i+7);
	}
	ofPopStyle();
}