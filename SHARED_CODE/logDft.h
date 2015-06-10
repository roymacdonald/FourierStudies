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

using namespace MusicalNotes;

class logDft {
public:
	logDft();
	~logDft();
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
