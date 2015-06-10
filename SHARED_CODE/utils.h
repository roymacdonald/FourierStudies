#pragma once
#include "ofConstants.h"

//--------------------------------------------------------------
inline float getMax(map<int, ofVec2f>& m){
	float tmax = m.begin()->second.x;
	for (map<int,ofVec2f>::iterator it=m.begin(); it!=m.end(); ++it){
		if (tmax < it->second.x) {
			tmax = it->second.x;
		}
	}
	return tmax;
}
//--------------------------------------------------------------
inline float getMax(float *a, size_t size){
    float tmax = FLT_MIN;
    if(size>0){
        tmax=a[0];
        for(int i=0; i<size; i++){
            if(tmax<a[i]){
                tmax=a[i];
            }
        }
    }
    return tmax;
}
inline float getMax(vector<float> a){
	getMax(a.data(), a.size());
}
//--------------------------------------------------------------
inline float getMax(vector<vector<float> > a){
    float tmax = FLT_MIN;
    if(a.size()>0){
        if (a[0].size()) {
            tmax=a[0][0];
            for(int i=0; i<a.size(); i++){
                float temp = getMax(a[i]);
                if(tmax<temp){
                    tmax=temp;
                }
            }
        }
    }
    return tmax;
}

//--------------------------------------------------------------
int sparseSize(vector< map<int, ofVec2f> > kernel){
	int m=0;
	for(int i = 0; i<kernel.size();i++){
		for (map<int,ofVec2f>::iterator it=kernel[i].begin(); it!=kernel[i].end(); ++it){
			if(it->first > m){
				m = it->first ;
			}
		}
	}
	return m;
}
//--------------------------------------------------------------
inline void subset( float* a, int start, int length, float * ret){
	memcpy(ret, a + start, sizeof(float) * length);
}
//--------------------------------------------------------------
inline vector<float> subset( vector<float> a, int start, int length){
    vector<float>temp (a.begin() + start, a.begin() + start + length);
    return temp;
}
//--------------------------------------------------------------
inline int nextpow2(int in){
    int i=1;
    while(true){
        i++;
        if(pow((double)2,i)>=in){
            return i;
        }
    }
}
//--------------------------------------------------------------
inline float log2 (float x) {
    return (log(x) / log(2));
}
