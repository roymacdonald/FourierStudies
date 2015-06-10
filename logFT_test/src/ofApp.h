#pragma once

#include "ofMain.h"
#include "logDft.h"
#include "logDftRenderer.h"
class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
	void cargar();
	void guiChangedF(float & f);
	void guiChangedI(int & i);
	void render();
	void renderBin();
    vector<float> audio;
	logDft lDFT;
	logDftRenderer renderer;
	bool bSetup;
	ofFbo fbo;
	ofRectangle waveRect, specRect, binRect;
	bool bDrawGui;
	int selectedBin;
};
