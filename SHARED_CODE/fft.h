//
//  fft2.h
//  emptyExample
//
//  Created by Roy Macdonald on 4/26/15.
//
//fft algorithm taken from processing lib minim
//

#pragma once
#include "ofMain.h"



class FFT{
public:
	FFT(){}
	~FFT(){
		delete [] rvrs;
		delete [] sinlookup;
		delete [] coslookup;
		delete [] real;
		delete [] imag;
		delete [] spectrum;
	}
	//----------------------------------------------------------------
	FFT(int ts) {
		set(ts);
	}
	//----------------------------------------------------------------
	void set(int ts){
		timeSize = ts;
		if((timeSize & timeSize - 1) != 0) {
			cout << "FFT: timeSize must be a power of two."<<endl;
		}
		else {
			rvrs = new int[timeSize];
			sinlookup = new float[timeSize];
			coslookup = new float[timeSize];
			buildReverseTable();
			buildTrigTables();
			spectrumSize = timeSize / 2 + 1;
		}
		allocateArrays();
	}
	//----------------------------------------------------------------
	const float* getSpectrum(){
		return spectrum;
	}
	//----------------------------------------------------------------
	void forward(float * buffer, size_t size) {
		if(size != timeSize) {
			cout << "FFT.forward: The length of the passed sample buffer must be equal to timeSize()." << endl;
            return;
        }
	///	cout << "Forward FFT real" << endl;
				clearArrays();
			bitReverseSamples(buffer);
			fft();
			fillSpectrum();
	}

	//----------------------------------------------------------------
	void forward(float * bufferReal, size_t sizeR, float * bufferImag, size_t sizeI) {
		if(sizeR != timeSize && sizeI != timeSize) {
			cout << "FFT.forward: The length of the passed sample buffer must be equal to timeSize()." << endl;
            return;
		}
	//	cout << "Forward FFT real imag" << endl;
				clearArrays();
			bitReverseSamples(bufferReal, bufferImag);
			fft();
			fillSpectrum();
	}
	//----------------------------------------------------------------
    void forward(vector<float> buffer) {
	//	cout << "FFT forward vector real" << endl;
		forward(&buffer[0], buffer.size());
	}
	//----------------------------------------------------------------
    void forward(vector<float> bufferReal, vector<float> bufferImag) {//buffer for complex numbers eg. buffer[buffersize][2];
	//cout << "FFT forward vector complex" << endl;
        forward(&bufferReal[0], bufferReal.size(), &bufferImag[0], bufferImag.size());
    }
	//----------------------------------------------------------------
	int getNumBins(){
		return spectrumSize;
	}
	//----------------------------------------------------------------
	float getReal(size_t i){
		return real[i];
	}
	//----------------------------------------------------------------
	float getImag(size_t i){
		return imag[i];
	}
protected:
	//----------------------------------------------------------------
	void allocateArrays() {
		spectrum = new float[spectrumSize];
		real = new float[timeSize];
		imag = new float[timeSize];
		clearArrays();
	}
	//----------------------------------------------------------------
	void clearArrays(){
		memset(real, 0, sizeof(float) * timeSize);
		memset(imag, 0, sizeof(float) * timeSize);
		memset(spectrum, 0, sizeof(float) * spectrumSize);
	}
	//----------------------------------------------------------------
	void fft() {
		for(int halfSize = 1; halfSize < timeSize; halfSize *= 2) {
			float phaseShiftStepR = coS(halfSize);
			float phaseShiftStepI = siN(halfSize);
			float currentPhaseShiftR = 1.0;
			float currentPhaseShiftI = 0.0;
			for(int fftStep = 0; fftStep < halfSize; fftStep++) {
				for(int i = fftStep; i < timeSize; i += 2 * halfSize) {
					int off = i + halfSize;
					float tr = currentPhaseShiftR * real[off] - currentPhaseShiftI * imag[off];
					float ti = currentPhaseShiftR * imag[off] + currentPhaseShiftI * real[off];
					real[off] = real[i] - tr;
					imag[off] = imag[i] - ti;
					real[i] += tr;
					imag[i] += ti;
				}
				float tmpR = currentPhaseShiftR;
				currentPhaseShiftR = tmpR * phaseShiftStepR - currentPhaseShiftI * phaseShiftStepI;
				currentPhaseShiftI = tmpR * phaseShiftStepI + currentPhaseShiftI * phaseShiftStepR;
			}
		}
	}
	//----------------------------------------------------------------
	void buildReverseTable() {
		int N = timeSize;
		rvrs[0] = 0;
		int lmt = 1;
		//println("N: "+N+"  N/2: "+ N/ 2);
		for(int bit = N/ 2; lmt < N; bit >>= 1) {
			for(int i = 0; i < lmt; i++)
				rvrs[i + lmt] = rvrs[i] + bit;
			lmt <<= 1;
		}
	}
	//----------------------------------------------------------------
	void bitReverseSamples(float * samplesR, float * samplesI) { //bit reverse complex
		for(int i = 0; i < timeSize; i++) {
			real[i] = samplesR[rvrs[i]];
			imag[i] = samplesI[rvrs[i]];
		}
	}
	//----------------------------------------------------------------
	void bitReverseSamples(float * samples) {// bit reverse real
		for(int i = 0; i < timeSize; i++) {
			real[i] = samples[rvrs[i]];
		}
	}
	//----------------------------------------------------------------
	float siN(int i) {
		return sinlookup[i];
	}
	//----------------------------------------------------------------
	float coS(int i) {
		return coslookup[i];
	}
	//----------------------------------------------------------------
	void buildTrigTables() {
		sinlookup[0]=0;//avoid division by 0
		coslookup[0]=0;
		for(int i = 1; i < timeSize; i++) {
			sinlookup[i] = sin(-PI /float(i));
			coslookup[i] = cos(-PI /float(i));
		}
	}
	//----------------------------------------------------------------
	void fillSpectrum() {
		for(int i = 0; i < spectrumSize; i++){
			spectrum[i] = (float)sqrt(real[i] * real[i] + imag[i] * imag[i]);
			if (isnan(spectrum[i]) || isinf(spectrum[i])) {
				cout << "spectrum nan or inf"<< endl;
			}
//		cout << "spectrum[" << i << "]: " << spectrum[i] << endl;
		}
	}
	//----------------------------------------------------------------
	int * rvrs;
	float* sinlookup;
	float* coslookup;
	float* real;
	float* imag;
	float* spectrum;
	int spectrumSize;
	int timeSize;
};

