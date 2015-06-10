//
//  ndft.h
//  emptyExample
//
//  Created by Roy Macdonald on 4/30/15.
//
//

#pragma once
#include "ofMain.h"
#include "utils.h"
#include "MusicalNotesUtils.h"
using namespace MusicalNotes;
class ndft {
public:
	ndft():refNoteFreq(440.0), bIsProcessed(false), bIsSetup(false), cuantos(0), procesadoSize(0){
		refNote = NOTE_A5;
	}
	//--------------------------------------------------------------
	~ndft(){
		delete [] spectrum;
		delete [] cosLUT;
		delete [] sinLUT;
		delete [] real;
		delete [] imag;
		delete [] procesado;
	}
	//--------------------------------------------------------------
	void setReferenceNoteFrequency(float freq){
		refNoteFreq = freq;
	}
	//--------------------------------------------------------------
	void setReferenceNote(Notes note){
		refNote = note;
	}
	//--------------------------------------------------------------
	void setup(Notes startNote, Notes endNote, int bufferSize, long sampRate = 44100, int binsPerNote=1, Notes refA = NOTE_A5, float refAfreq = 440){
		bIsProcessed = false;
		this->startNote = startNote;
		this->endNote = endNote;
		this->bufferSize = bufferSize;
		this->binsPerNote = binsPerNote;
		this->sampRate = sampRate;
		this->refNote = refA;
		this->refNoteFreq = refAfreq;
		totalBins =((int)endNote - (int)startNote)*binsPerNote;
		LUTSize = totalBins*bufferSize;
		cout << "LUTSize: " << LUTSize << endl;
		if (bIsSetup) {
			delete [] cosLUT;
			delete [] sinLUT;
			delete [] imag;
			delete [] real;
			delete [] spectrum;
			delete [] procesado;
		}
		cosLUT = new float[LUTSize];
		sinLUT = new float[LUTSize];
		real = new float [totalBins];
		imag = new float [totalBins];
		spectrum = new float [totalBins];
		clearMem();
		buildLUTs();
		bIsSetup = true;
		maximo =0;
	}
	//--------------------------------------------------------------
	void process(float * samples, int sampSize){
		if (bIsSetup) {
			clearMem();
			cout << "samples max: " << getMax(samples, sampSize)<< endl;
			if (sampSize == bufferSize) {
//				cout << "bufferSize: " << bufferSize << " totalBins: " << totalBins << endl;
				for (int i =0; i < totalBins; i++) {
					real[i]=0;
					imag[i]=0;
					for (int b = 0; b < bufferSize; b++) {
						long lp = i*bufferSize+b;
						real [i] += samples[b]* cosLUT[lp];
						imag [i] += samples[b]* -sinLUT[lp];
						//if ( samples[b] != 0.0f){//rr != 0 || ii != 0) {
						///cout << "rr: " << rr << "  " << samples[b]* cosLUT[lp] << " ii: " << ii << " samples[" << i << "]: " << samples[i] << "sL["<< lp<< "]: " << sinLUT[lp] << "  " << cosLUT[i] << endl;
						//}
						//if (real[i] != 0 || imag[i] != 0) {
						//	cout << "real["<<i<< "]: " << real[i] << "  imag: " << imag[i]<<endl;
						//}
						bool bPrint = false;
						if(isnan(real[i])){
							cout << "real["<<i<<"]: is nan.";
							bPrint = true;
						}
						if(isinf(real[i])){
							cout << "real["<<i<<"]: is inf.";
							bPrint = true;
						}
						if(isnan(imag[i])){
							cout << "imag["<<i<<"]: is nan.";
							bPrint = true;
						}
						if(isinf(imag[i])){
							cout << "imag["<<i<<"]: is inf.";
							bPrint = true;
						}
						if (bPrint) {
							cout << "sample[" << b << "]: " << samples[b];
							cout << " cosLUT["<< lp <<"]: " << cosLUT[lp];
							cout << " sinLUT["<< lp <<"]: " << sinLUT[lp];
							cout <<" real: " <<real[i];
							cout <<" imag: " << imag[i];
							cout << " samples[b]* cosLUT[lp]: " << samples[b]* cosLUT[lp];
							cout << " samples[b]* -sinLUT[lp]: " << samples[b]* -sinLUT[lp];
							
							cout << endl;
							
						}
					}
					//cout << "real["<<i<< "]: " << real[i] << "  imag: " << imag[i]<<endl;
				}
				fillSpectrum();
				bIsProcessed = true;
				//	cout << "ndft:: proccesed!" << endl;
			}
		}
	}
	//--------------------------------------------------------------
	void processAll(vector<float> data, bool bOverlaped){
		if (bIsSetup && data.size() > 0) {
			cout << "processAll data size: " << data.size() << endl;
			
			cuantos = bOverlaped?ofGetWidth()-1:ceil(float(data.size())/bufferSize);
			
			procesado = new float [cuantos * totalBins];
			procesadoSize = cuantos * totalBins;
			memset(procesado, 0, procesadoSize * sizeof(float));
			cout << "ProcessData cuantos: " << cuantos << endl;
			int inc = bOverlaped?floor(data.size()/float(ofGetWidth())):bufferSize;
			cout << "inc: " << inc << endl;
			float *sset = new float [bufferSize];
			
			for(int i=0; i<cuantos; i++){
				memset(sset, 0, bufferSize* sizeof(float));
				if(data.size()-(i*inc)<bufferSize){
			//		cout << "dataSize: " << data.size() << " i*inc: " << i*inc << endl;
					subset(data.data(),i*inc,data.size()-(i*inc)-1, sset);
				}else{
					subset(data.data(),i*inc,bufferSize, sset);
				}
				//if(bApplyWindow){
				//	window.apply(sset, bufferSize);
				//}
				process(sset, bufferSize);
				
				for(int j =1; j<totalBins;j++){
					procesado[i*totalBins + j]=getSpectrum()[j];
				}
				
				//	porcentaje =floor(100*float(i)/(cuantos-1));
				//	cout << porcentaje << endl;
			}
			delete [] sset;
			maximo= getMax(procesado, cuantos * getNumBins());
			cout << "Maximo: " << maximo << endl;
			bIsProcessed=true;
		}else{
			cout << "CAN'T PROCESS DATA IF NOT SETUP!" << endl;
		}
	}
	//--------------------------------------------------------------
	int getBufferSize(){
		return bufferSize;
	}
	//--------------------------------------------------------------
	int getCuantos(){
		return cuantos;
	}
	//--------------------------------------------------------------
	int getNumBins(){
		return totalBins;
	}
	//--------------------------------------------------------------
	float * getSpectrum(){
		return spectrum;
	}
	float getMaximo(){
		return maximo;
	}
	bool isProcessed(){return bIsProcessed;}
	bool isSetup(){return bIsSetup;}
	float * getProcessed(){
		return procesado;
	}
	float * getReal(){
		return real;
	}
	float * getCosLUT(){return cosLUT;}
	float * getSinLUT(){return sinLUT;}
	Notes getEndNote(){return endNote;}
	Notes getStartNote(){return startNote;}
	int procesadoSize;
private:
	//--------------------------------------------------------------
	void fillSpectrum(){
		for (int i =0; i < totalBins; i++) {
			spectrum[i] = sqrt(real[i]*real[i] + imag[i]*imag[i]);
			if(isnan(spectrum[i]) ){
				cout << "spectrum["<<i<<"]: " << "is nan. real: " <<real[i] <<" imag: " << imag[i] << "  sqrt: " << sqrt(real[i]*real[i] + imag[i]*imag[i])<< endl;
			}
			if( isinf(spectrum[i])){
				cout << "spectrum["<<i<<"]: " << "is inf. real: " <<real[i] <<" imag: " << imag[i] << "  sqrt: " << sqrt(real[i]*real[i] + imag[i]*imag[i])<< endl;
			}
		}
	}
	//--------------------------------------------------------------
	void clearMem(){
		memset(real, 0, totalBins);
		memset(imag, 0, totalBins);
	}
	//--------------------------------------------------------------
	void buildLUTs(){
		cout << "buildLUTs" << endl;
		memset(cosLUT, 0, LUTSize);
		memset(sinLUT, 0, LUTSize);
		for (int i =0; i < totalBins; i++) {
			//Notes n = Notes(i + (int)startNote);
			//					cout << "Note: " << n << "  Freq: " << noteToFreq(Notes(i + (int)startNote)) << endl;
			for (int b = 0; b < bufferSize; b++) {
				cosLUT[i*bufferSize + b]= createCosWave(noteToFreq(Notes(i + (int)startNote)), sampRate, b);
				sinLUT[i*bufferSize + b]= createSinWave(noteToFreq(Notes(i + (int)startNote)), sampRate, b);
				//	cout << cosLUT[i*bufferSize + b] << endl;
				if(isnan(cosLUT[i*bufferSize + b]) || isinf(cosLUT[i*bufferSize + b])){
					cout << "cosLUT["<<i<<"]: " << "is nan or inf. " << endl;
				}
				if(isnan(sinLUT[i*bufferSize + b]) || isinf(sinLUT[i*bufferSize + b])){
					cout << "sinLUT["<<i<<"]: " << "is nan or inf. " << endl;
				}
			}
		}
		//		cout << "LUTs built!" << endl;
	}
	//--------------------------------------------------------------
	int totalBins, LUTSize;
	long sampRate;
	int binsPerNote, bufferSize;
	Notes startNote, endNote;
	
	float* spectrum;
	float* cosLUT;
	float* sinLUT;
	float* real;
	float* imag;
	float* procesado;
	
	float maximo;
	int cuantos;
	
	Notes refNote;//default A5
	float refNoteFreq;//default 440
	bool bIsProcessed, bIsSetup;
	
	//vector<float> data;
	
};
