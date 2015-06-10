//
//  logDftRenderer.cpp
//  emptyExample
//
//  Created by Roy Macdonald on 4/30/15.
//
//

#include "logDftRenderer.h"


//--------------------------------------------------------------
void logDftRenderer::draw(ofRectangle r, bool bDrawBands){
	ofFbo fbo;
	fbo.allocate(r.width, logDftPtr->getNumBins(), GL_RGBA);
	fbo.begin();
	ofClear(0, 0);
	drawVector(ofRectangle(0, 0, r.width, logDftPtr->getNumBins()), bDrawBands);
	fbo.end();
	fbo.draw(r);
}
//--------------------------------------------------------------
void logDftRenderer::drawVector(ofRectangle r, bool bDrawBands){
	if(logDftPtr->isProcessed()){
		ofMesh m;
		m.setMode(OF_PRIMITIVE_LINES);
		float yInc = r.height/logDftPtr->getNumBins();
		vector< vector <float> >& spec = logDftPtr->getSpectrum();
		for(int j =0; j<logDftPtr->getNumBins();j++){
			for(int i=1; i<spec[j].size(); i++){
				float xInc = r.width/spec[j].size();
				m.addVertex(ofVec3f((i -1) * xInc, j * yInc));
				m.addColor(ofFloatColor(1.0, spec[j][i-1]/logDftPtr->getMaximum()*multData));
				m.addVertex(ofVec3f(i * xInc, j * yInc));
				m.addColor(ofFloatColor(1.0, spec[j][i]/logDftPtr->getMaximum()*multData));
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
void logDftRenderer::drawBands(ofRectangle r){
	int noteOffSet = (logDftPtr->getStartNote()%12);
	bool  sharps [] = {false,true,false,false,true,false,true,false,false,true,false,true};
	string notes []= {"A","","B","C","","D","","E","F","","G",""};
	
	float inc = r.height/(logDftPtr->getEndNote() - logDftPtr->getStartNote());
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