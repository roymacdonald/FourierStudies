//DFT algorith taken from minim
// https://github.com/ddf/Minim/blob/master/src/ddf/minim/analysis/DFT.java

#pragma once
#include "ofMain.h"

class DFT{
public:
	//----------------------------------------------------------------
	DFT(){}
	DFT(int ts){
		set(ts);
	}
	//----------------------------------------------------------------
	void set(int ts){
		timeSize = ts;
		if (timeSize % 2 != 0){
			cout << "DFT: timeSize must be even." << endl;
			return;
		}
		allocateArrays();
		buildTrigTables();
	}
	//----------------------------------------------------------------
	~DFT(){
		delete [] sinlookup;
		delete [] coslookup;
		delete [] real;
		delete [] imag;
		delete [] spectrum;
	}
	//----------------------------------------------------------------
	void forward(float* samples, int size){
		forward(samples, samples, size, false);
	}
	//----------------------------------------------------------------
	void forward(float* samplesR, float* samplesI, int size, bool bIsComplex = true){
		if (size != timeSize){
			cout << "DFT.forward: The length of the passed sample buffer must be equal to DFT.timeSize()." << endl;
			return;
		}
		cout << "dft forward"<<endl;
		//if (bIsComplex) {
		//	doWindow(samplesI, size);
		//}
		//doWindow(samplesR, size);
		clearArrays();
		int N = size;
		for (int f = 0; f <= N / 2; f++){
			real[f] = 0.0f;
			imag[f] = 0.0f;
			for (int t = 0; t < N; t++){
				real[f] += samplesR[t] * coS(t * f);
				imag[f] += samplesR[t] * -siN(t * f);
			}
		}
		fillSpectrum();
	}
	//----------------------------------------------------------------
	void inverse(float* buffer, int size){
		int N = size;
		real[0] /= N;
		imag[0] = -imag[0] / (N / 2);
		real[N / 2] /= N;
		imag[N / 2] = -imag[0] / (N / 2);
		for (int i = 0; i < N / 2; i++){
			real[i] /= (N / 2);
			imag[i] = -imag[i] / (N / 2);
		}
		for (int t = 0; t < N; t++){
			buffer[t] = 0.0f;
			for (int f = 0; f < N / 2; f++){
				buffer[t] += real[f] * cos(t * f) + imag[f] * sin(t * f);
			}
		}
	}
	
	//----------------------------------------------------------------
	int getNumBins(){return spectrumSize;}
	//----------------------------------------------------------------
	float* getSpectrum(){return spectrum;}
	//----------------------------------------------------------------
	float getReal(size_t i){return real[i];}
	//----------------------------------------------------------------
	float getImag(size_t i){return imag[i];}
	//----------------------------------------------------------------
protected:
	//----------------------------------------------------------------
	void fillSpectrum(){
		for (int i = 0; i < spectrumSize; i++){
			spectrum[i] = (float) sqrt(real[i] * real[i] + imag[i] * imag[i]);
		}
	}
	//----------------------------------------------------------------
	void allocateArrays(){
		spectrumSize = timeSize / 2 + 1;
		spectrum = new float[spectrumSize];
		real = new float[spectrumSize];
		imag = new float[spectrumSize];
		clearArrays();
	}
	//----------------------------------------------------------------
	void clearArrays(){
		memset(spectrum, 0, spectrumSize);
		memset(real, 0, spectrumSize);
		memset(imag, 0, spectrumSize);
	}
	
private:
	
	int timeSize;
	int spectrumSize;
	float* sinlookup;
	float* coslookup;
	float* real;
	float* imag;
	float* spectrum;
	//----------------------------------------------------------------
	void buildTrigTables(){
		cout << "dft buildTrigTables...";
		int N = spectrumSize * timeSize;
		sinlookup = new float[N];
		coslookup = new float[N];
		for (int i = 0; i < N; i++)
		{
			sinlookup[i] = (float) sin(i * TWO_PI / timeSize);
			coslookup[i] = (float) cos(i * TWO_PI / timeSize);
		}
		cout << "done." << endl;
	}
	//----------------------------------------------------------------
	float siN(int i){
		return sinlookup[i];
	}
	//----------------------------------------------------------------
	float coS(int i){
		return coslookup[i];
	}
};
