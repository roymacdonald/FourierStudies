#pragma once

#include "ofMain.h"
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

	void fillBuffer(float *b, int bSize, float freq, int sampRate, bool bSin =false);
	void calcBandwidth();
	
	float* buffer;
	int bufferSize, maxBufferSize, sampleRate, numNotes;
	float bufferFreq;
	int bufferFreqIndex;
	
	
};
