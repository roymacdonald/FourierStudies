//
//  logDft.cpp
//  emptyExample
//
//  Created by Roy Macdonald on 09-05-15.
//
//

#include "logDft.h"

//
//  logDft.h
//  emptyExample
//
//  Created by Roy Macdonald on 08-05-15.
//
//



logDft::logDft():refNoteFreq(440.0), bIsProcessed(false), bIsSetup(false){
		refNote = NOTE_A5;
	}
	//--------------------------------------------------------------
	logDft::~logDft(){
//		delete [] spectrum;
		delete [] cosLUT;
		delete [] sinLUT;
	//	delete [] real;
	//	delete [] imag;
		delete [] indLut;
		delete [] sizeLut;
		delete [] binFreqs;
	}
	//--------------------------------------------------------------
	float logDft::noteToFreq(Notes note){
		return noteFreq(note, refNote, refNoteFreq);
	}
	//--------------------------------------------------------------
	int logDft::freqToBinSize(float freq, int sampleRate){
		return (int)floor(sampleRate/freq);
	}
	//--------------------------------------------------------------
	void logDft::setReferenceNoteFrequency(float freq){
		this->refNoteFreq = freq;
	}
	//--------------------------------------------------------------
	void logDft::setReferenceNote(Notes note){
		refNote = note;
	}
	//--------------------------------------------------------------
	void logDft::setup(Notes startNote, Notes endNote, long sampRate, int binsPerNote, Notes refA, float refAfreq){
		bIsProcessed = false;
		this->startNote = startNote;
		this->endNote = endNote;
		this->binsPerNote = binsPerNote;
		this->sampRate = sampRate;
		this->refNote = refA;
		this->refNoteFreq = refAfreq;
		totalBins =((int)endNote - (int)startNote);//*binsPerNote;
		cout << "totalBins: " << totalBins << endl;
		if (bIsSetup) {
			delete [] cosLUT;
			delete [] sinLUT;
	//		delete [] imag;
	//		delete [] real;
		//	delete [] spectrum;
			delete [] indLut;
			delete [] sizeLut;
			delete [] binFreqs;
		}
		
		indLut = new int[totalBins];
		sizeLut = new int[totalBins];
		binFreqs = new float[totalBins];
		int accumSize =0;
		for (int i =0; i < totalBins; i++) {
			binFreqs[i] = noteToFreq(Notes(i+startNote));
			sizeLut[i] = freqToBinSize(binFreqs[i]);
			indLut[i] = accumSize;
			accumSize+=sizeLut[i];
		}
		LUTSize = accumSize;
		cosLUT = new float[LUTSize];
		sinLUT = new float[LUTSize];
//		real = new float [totalBins];
//		imag = new float [totalBins];
	//	spectrum = new float [totalBins];
		
	//	clearMem();
		buildLUTs();
		bIsSetup = true;
		maximo =0;
	}
	//--------------------------------------------------------------
	float logDft::multLut(float* lut,  int lutSize, float * data, float* winLut){
		float accum = 0;
		for (int i = 0; i < lutSize; i++) {
			accum+=lut[i]*data[i]*(0.53836-(0.46164* winLut[i]));
			
			if(isinf(lut[i])){
				cout << "MultLut:lut["<<i<<"] is inf"<<endl;
			}if(isinf(data[i])){
				cout << "MultLut:data["<<i<<"] is inf"<<endl;
			}
			if(isnan(data[i])){
				cout << "MultLut:data["<<i<<"] is nan"<<endl;
			}
			if(isnan(lut[i])){
				cout << "MultLut:lut["<<i<<"] is nan"<<endl;
			}
			if(isinf(lut[i]*data[i])){
				cout << "MultLut:lut[i]*data["<<i<<"] is inf"<<endl;
			}
			if(isnan(lut[i]*data[i])){
				cout << "MultLut:lut[i]*data["<<i<<"] is nan"<<endl;
			}
			if(isinf(accum)){
				cout << "MultLut:accum["<<i<<"] is inf"<<endl;
			}
			if(isnan(accum)){
				cout << "MultLut:accum["<<i<<"] is nan"<<endl;
			}
			
		}
		return accum;
	}
	//--------------------------------------------------------------
	void logDft::process(float * samples, int sampSize){
		if (bIsSetup) {
			unsigned long long startTime = ofGetElapsedTimeMicros();
			clearMem();
				for (int i =0; i < totalBins; i++) {
					unsigned int b = 0;
					vector<float>realTemp;
					vector<float>imagTemp;
					for (; b + sizeLut[i] < sampSize; b+=sizeLut[i]) {
						realTemp.push_back(multLut(&cosLUT[indLut[i]], sizeLut[i], &samples[b], &cosLUT[indLut[i]]));
						imagTemp.push_back(-multLut(&sinLUT[indLut[i]], sizeLut[i], &samples[b], &cosLUT[indLut[i]]));
					}
					/*
					if (b < sampSize) {
						float * temp = new float [sizeLut[i]];
						memset(temp, 0, sizeLut[i]);
						memcpy(temp, &samples[b], sampSize-b);
						realTemp.push_back(multLut(&cosLUT[indLut[i]], sizeLut[i], temp));
						imagTemp.push_back(multLut(&sinLUT[indLut[i]], sizeLut[i], temp));
					}
					//*/
					real.push_back(realTemp);
					imag.push_back(imagTemp);
				}
				fillSpectrum();
				bIsProcessed = true;
			unsigned long long t = ofGetElapsedTimeMicros() - startTime;
			cout << "process done. Time taken: " << t << " for " << sampSize << " samples. Maximum: " << maximo<< endl;
			}
		}
//--------------------------------------------------------------
float logDft::createCosWave(float* bin, float freq, int sampRate, int size){
	for (int i = 0; i < size; i++) {
		bin[i] = cos(TWO_PI*i*freq/sampRate);
	}
}
//--------------------------------------------------------------
float logDft::createSinWave(float* bin, float freq, int sampRate, int size){
	for (int i = 0; i < size; i++) {
		bin[i] = sin(TWO_PI*i*freq/sampRate);
	}
}
//--------------------------------------------------------------
float logDft::noteFreq(Notes note, Notes refNote, float refNoteFreq){
	return pow(pow(2, 1/12.0f), (int)note - (int)refNote) * refNoteFreq;
}
	//--------------------------------------------------------------
	//---------GETTERS--------------------------
int logDft::getMaxBin(){
	if(bIsProcessed){
		return spectrum[totalBins -1].size();
	}else return 0;
}
	int logDft::getNumBins(){return totalBins;}
	vector< vector <float> >& logDft::getSpectrum(){return spectrum;}
	float logDft::getMaximum(){return maximo;}
	bool logDft::isProcessed(){return bIsProcessed;}
	bool logDft::isSetup(){return bIsSetup;}
	vector< vector <float> >&  logDft::getReal(){return real;}
	vector< vector <float> >&  logDft::getImag(){return imag;}
	float * logDft::getCosLUT(){return cosLUT;}
	float * logDft::getSinLUT(){return sinLUT;}
	Notes logDft::getEndNote(){return endNote;}
	Notes logDft::getStartNote(){return startNote;}

//--------------------------------------------------------------
	void logDft::fillSpectrum(){
		maximo = 0;
		for (int i =0; i < real.size(); i++) {
			vector<float>temp;
			for (int j=0; j<real[i].size(); j++) {
				temp.push_back( sqrt(real[i][j]*real[i][j] + imag[i][j]*imag[i][j]));
				if (maximo < temp.back()) {
					maximo = temp.back();
				}
			}
			spectrum.push_back(temp);
		}
	}
	//--------------------------------------------------------------
	void logDft::clearMem(){
			spectrum.clear();
			real.clear();
			imag.clear();
	}
	//--------------------------------------------------------------
	void logDft::buildLUTs(){
		cout << "buildLUTs" << endl;
		memset(cosLUT, 0, LUTSize);
		memset(sinLUT, 0, LUTSize);
		for (int i =0; i < totalBins; i++) {
			createCosWave(cosLUT+ indLut[i], binFreqs[i], sampRate, sizeLut[i]);
			createSinWave(sinLUT+ indLut[i], binFreqs[i], sampRate, sizeLut[i]);
		}
		for (int i =0; i < LUTSize; i++) {
			if(isnan(cosLUT[i])){
				cout << "cosLut["<<i<<"] is nan"<<endl;
			}
			if(isnan(sinLUT[i])){
				cout << "sinLut["<<i<<"] is nan"<<endl;
			}
			if(isinf(cosLUT[i])){
				cout << "cosLut["<<i<<"] is inf"<<endl;
			}
			if(isinf(sinLUT[i])){
				cout << "sinLut["<<i<<"] is inf"<<endl;
			}
		}
	}
