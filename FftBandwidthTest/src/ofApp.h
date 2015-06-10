#pragma once

#include "ofMain.h"
#include "windowing.h"
#include "fft.h"
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
	void fillBuffer(float *b, int bSize, float freq, bool bSin =false);
	void calcBandwidth();
	
	float* buffer;
	float* bw;
	float* bwS;
	float* bwH;
	float* freqs;
	int bufferSize, sampRate, binNum;
	float bufferFreq, bwMult, bwWidth, minFreq, maxFreq;

	unsigned long lastClick;
	
	ofRectangle r;
	
	FFTX fft;
	
};
