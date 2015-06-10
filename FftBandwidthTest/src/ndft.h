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
enum Notes{
	NOTE_C0 = 0, NOTE_Db0, NOTE_D0, NOTE_Eb0, NOTE_E0, NOTE_F0, NOTE_Gb0, NOTE_G0, NOTE_Ab0, NOTE_A0, NOTE_Bb0, NOTE_B0,
	NOTE_C1, NOTE_Db1, NOTE_D1, NOTE_Eb1, NOTE_E1, NOTE_F1, NOTE_Gb1, NOTE_G1, NOTE_Ab1, NOTE_A1, NOTE_Bb1, NOTE_B1,
	NOTE_C2, NOTE_Db2, NOTE_D2, NOTE_Eb2, NOTE_E2, NOTE_F2, NOTE_Gb2, NOTE_G2, NOTE_Ab2, NOTE_A2, NOTE_Bb2, NOTE_B2,
	NOTE_C3, NOTE_Db3, NOTE_D3, NOTE_Eb3, NOTE_E3, NOTE_F3, NOTE_Gb3, NOTE_G3, NOTE_Ab3, NOTE_A3, NOTE_Bb3, NOTE_B3,
	NOTE_C4, NOTE_Db4, NOTE_D4, NOTE_Eb4, NOTE_E4, NOTE_F4, NOTE_Gb4, NOTE_G4, NOTE_Ab4, NOTE_A4, NOTE_Bb4, NOTE_B4,
	NOTE_C5, NOTE_Db5, NOTE_D5, NOTE_Eb5, NOTE_E5, NOTE_F5, NOTE_Gb5, NOTE_G5, NOTE_Ab5, NOTE_A5, NOTE_Bb5, NOTE_B5,
	NOTE_C6, NOTE_Db6, NOTE_D6, NOTE_Eb6, NOTE_E6, NOTE_F6, NOTE_Gb6, NOTE_G6, NOTE_Ab6, NOTE_A6, NOTE_Bb6, NOTE_B6,
	NOTE_C7, NOTE_Db7, NOTE_D7, NOTE_Eb7, NOTE_E7, NOTE_F7, NOTE_Gb7, NOTE_G7, NOTE_Ab7, NOTE_A7, NOTE_Bb7, NOTE_B7,
	NOTE_C8, NOTE_Db8, NOTE_D8, NOTE_Eb8, NOTE_E8, NOTE_F8, NOTE_Gb8, NOTE_G8, NOTE_Ab8, NOTE_A8, NOTE_Bb8, NOTE_B8
};


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
	static float createCosWave(float freq, int sampRate, int sampNumber){
		return cos(TWO_PI*sampNumber*freq/sampRate);
	}
	//--------------------------------------------------------------
	static float createSinWave(float freq, int sampRate, int sampNumber){
		return sin(TWO_PI*sampNumber*freq/sampRate);
	}
	//--------------------------------------------------------------
	float noteToFreq(Notes note){
		//	cout << "NoteToFreq: n: " << (int)note - (int)refNote << " refNoteFreq: " << refNoteFreq << endl;
		return pow(pow(2, 1/12.0f), (int)note - (int)refNote) * refNoteFreq;
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
