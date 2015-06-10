#pragma once

#include "ofMain.h"
#include "windowing.h"

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
	float cosineIntegral(float x, float a1, float a2);
	void fillBuffer(float *b, int bSize, float freq, bool bSin =false);
	void sumAll();
	void calcBandwidth();
	
	float* buffer;
	float* lut;
	float* lutS;
	float* bw;
	float* bwS;
	float* bwH;
	float* freqs;
	int bufferSize, sampRate;
	float sum, bufferFreq, lutFreq, bwMult, bwWidth, minFreq, maxFreq;

	unsigned long lastClick;
	
	bool bWindow;
	ofRectangle r;
	
	hammingWindow window;
	
	int lutMinMargin, lutMaxMargin;
	
};
