//
//  ndftRenderer.h
//  emptyExample
//
//  Created by Roy Macdonald on 4/30/15.
//
//

#pragma once
#include "ofMain.h"
#include "ndft.h"
#include "ofxGui.h"

class ndftRenderer{
public:
	ndftRenderer(){}
	void setup(ndft& n){
		ndftPtr = &n;
		gui.setup();
		gui.add(multData.set("data multiplier", 1, 0, 1));
		gui.add(bandsOpacity.set("bands opacity", 100, 0, 255));
	}
	void draw(ofRectangle r, bool bDrawBands = true);
	void drawVector(ofRectangle r, bool bDrawBands = true);
	void drawBands(ofRectangle r);
	void drawGui(){
		gui.draw();
	}
	
	ndft * ndftPtr;
	
	ofParameter<float>multData;
	ofParameter<int> bandsOpacity;
	ofxPanel gui;

};