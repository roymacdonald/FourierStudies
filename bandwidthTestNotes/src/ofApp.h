#pragma once

#include "ofMain.h"
#include "windowing.h"
#include "MusicalNotesUtils.h"

using namespace MusicalNotes;

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
	float* lut;
	float* lutS;
	float* bw;
	float* bwS;
	float* bwH;
	float* freqs;
	int bufferSize, sampRate;
	float sum, bufferFreq, lutFreq, bwMult, bwWidth;
	
	Notes minNote, maxNote;
	int numNotes;

	unsigned long lastClick;
	
	bool bWindow;
	ofRectangle r;
	
	hammingWindow window;
	
	int lutMinMargin, lutMaxMargin;
	int lutFreqIndex, bufferFreqIndex;
	
	
};
