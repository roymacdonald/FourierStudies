//
//  logDft.h
//  emptyExample
//
//  Created by Roy Macdonald on 08-05-15.
//
//

#pragma once
#include "ofMain.h"

#include "MusicalNotesUtils.h"
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



class logDft {
public:
	logDft();
	~logDft();
	float noteToFreq(Notes note);
	static int freqToBinSize(float freq, int sampleRate = 44100);
	void setReferenceNoteFrequency(float freq);
	void setReferenceNote(Notes note);
	void setup(Notes startNote, Notes endNote, long sampRate = 44100, int binsPerNote=1, Notes refA = NOTE_A5, float refAfreq = 440);
	void process(float * samples, int sampSize);
	int getNumBins();
	vector< vector <float> >& getSpectrum();
	float getMaximum();
	bool isProcessed();
	bool isSetup();
	vector< vector <float> >&  getReal();
	vector< vector <float> >&  getImag();
	float * getCosLUT();
	float * getSinLUT();
	int getMaxBin();
	Notes getEndNote();
	Notes getStartNote();
	
	
	float createCosWave(float* bin, float freq, int sampRate, int size);
	float createSinWave(float* bin, float freq, int sampRate, int size);
	float noteFreq(Notes note, Notes refNote, float refNoteFreq);


private:
	void fillSpectrum();
	void clearMem();
	void buildLUTs();
	
	float multLut(float* lut, int lutSize, float * data, float* winLut);
	
	int totalBins, LUTSize;
	long sampRate;
	int binsPerNote;
	Notes startNote, endNote;
	vector< vector <float> > spectrum;
	vector< vector <float> > real;
	vector< vector <float> > imag;
	float* cosLUT;
	float* sinLUT;

	
	int* indLut;
	int* sizeLut;
	float* binFreqs;
	float maximo;
	Notes refNote;//default A5
	float refNoteFreq;//default 440
	bool bIsProcessed, bIsSetup;
	
	};
