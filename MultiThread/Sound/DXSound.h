#ifndef DXSound_H
#define DXSound_H

#include "Waveform.h"
#include <d3dx9.h>

class DXSound
{
public:
	DXSound();
	~DXSound();
	
	//Loads a wav file
	void LoadDXSound(char *soundLoad, HWND handle);
	//Cleans DXSound
	void CleanDXSound();
	//Set Sound ID
	void SetDXSoundID(int newID);
	LPDIRECTSOUNDBUFFER GetDXSoundBuffer();

	//Return Sound ID
	int GetDXSoundID();

private:
	//Sound ID
	int mDXSoundID;
	//DirectSound Object
	LPDIRECTSOUND mDXSoundObj;
	//DirectSound Buffer
	LPDIRECTSOUNDBUFFER mDXSoundBuffer;
	//Waveform Object
	Waveform mWaveformObj;
	//Direct Sound Buffer Description
	DSBUFFERDESC mDXSoundBufferDesc;
};

#endif