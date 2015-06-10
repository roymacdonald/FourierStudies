//
//  MusicalNotesUtils.h
//  emptyExample
//
//  Created by Roy Macdonald on 09-05-15.
//
//
#pragma once

namespace MusicalNotes {
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
	
	//--------------------------------------------------------------
	static int getBinSizeFromFreq(float freq, int sampleRate){
		return (int)round(float(sampleRate)/freq);
	}
	//--------------------------------------------------------------
	static float createCosWave(float* bin, float freq, int sampRate, int size){
		float f =(freq/sampRate); // = 1/bufferSize
		f = 1/f; // = bufferSize
		f -= 1;  // = bufferSize -1
		f = 1/f; // = 1/(bufferSize -1)
		
		for (int i = 0; i < size; i++) {
		//	freq = sampleRate / (float)bufferSize;
		//	bin[i] = cos(TWO_PI*i/(float)(size-1));
			bin[i] = cos(TWO_PI*i*f);
		}
	}
	//--------------------------------------------------------------
	static float createSinWave(float* bin, float freq, int sampRate, int size){
		float f =(freq/sampRate); // = 1/bufferSize
		f = 1/f; // = bufferSize
		f -= 1;  // = bufferSize -1
		f = 1/f; // = 1/(bufferSize -1)

		for (int i = 0; i < size; i++) {
			bin[i] = sin(TWO_PI*i*f);
		}
	}
	//--------------------------------------------------------------
	static float getNoteFreq(Notes note, Notes refNote = NOTE_A5, float refNoteFreq = 440.0f){
		return pow(pow(2, 1/12.0f), (int)note - (int)refNote) * refNoteFreq;
	}
};