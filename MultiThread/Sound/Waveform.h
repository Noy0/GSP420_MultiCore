#ifndef Waveform_H
#define Waveform_H

#include <windows.h>
#include "dsound.h"

class Waveform
{
public:
	//Waveform constructor
	Waveform();
	//Waveform destructor
	~Waveform();

	//Releases memory
	void CleanWaveform();
	//Loads a specifed .wav file
	void LoadWaveform(char *cWavfile);
	//Returns length of wav file
	DWORD GetWaveformLength();
	//Returns the data contained in wav file
	unsigned char* GetWaveformData();
	//Returns the Waveform
	LPWAVEFORMATEX GetWaveform();

private:
	//Waveform
	LPWAVEFORMATEX mlpWaveform;
	//Data contained in wav file
	unsigned char* mucWaveformData;
	//Length of data within file
	DWORD mdwWaveformLength;
};

#endif