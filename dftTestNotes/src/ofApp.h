#pragma once

#include "ofMain.h"
#include "ndft.h"
#include "ndftRenderer.h"
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
	void processAll(vector<float> a, bool overlap);
	
    vector<float> audio;
	float* buffer;
	ndft NDFT;
	ndftRenderer renderer;
	bool bSetup;
	ofFbo fbo;
	int bufferSize, sampRate, binNum;
};
