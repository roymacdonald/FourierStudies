//
//  windowing.h
//  emptyExample
//
//  Created by Roy Macdonald on 4/25/15.
//
//

#pragma once
#include "ofMain.h"

class windowing{
public:
	windowing(){}
	virtual ~windowing(){}
	virtual void setup(int size)=0;
	float& operator[]( int n ){
		return window[n];
	}
	float * apply(float * samples, size_t size){
		if (bCreated) {
			for(int i=0; i<size; i++){
				samples[i]*=window[i];
			}
		}
		return samples;
	}
	vector <float> & apply(vector <float> & samples){
		if (bCreated) {
			for(int i=0; i<samples.size(); i++){
				samples[i]*=window[i];
			}
		}
		return samples;
	}
	vector<float> window;
	bool bCreated = false;
};


class hammingWindow: public windowing{
public:
	hammingWindow(){}
	hammingWindow(int size){
		setup(size);
	}
	void setup(int size){
		window.resize(size);
		for(int i = 0; i<size;i++){
			window[i]= 0.53836-(0.46164*cos(TWO_PI*i/(size-1)));
		}
		bCreated = true;
	}
};

