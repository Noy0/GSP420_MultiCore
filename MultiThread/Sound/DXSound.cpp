#include "DXSound.h"
//#include "WindowBase.h"

DXSound::DXSound()
{
	//
}

DXSound::~DXSound()
{
	CleanDXSound();
}

void DXSound::LoadDXSound(char *soundLoad, HWND handle)
{
	//Loads a .wav file
	mWaveformObj.LoadWaveform(soundLoad);
	//Creates the device
	DirectSoundCreate(NULL, &mDXSoundObj, NULL);
	mDXSoundObj->SetCooperativeLevel(handle, DSSCL_NORMAL);
	
	//Defaults the Buffer Description
	ZeroMemory(&mDXSoundBufferDesc, sizeof(DSBUFFERDESC));

	
	mDXSoundBufferDesc.dwBufferBytes = mWaveformObj.GetWaveformLength();//DSBCAPS_LOCSOFTWARE |
	mDXSoundBufferDesc.dwFlags =  DSBCAPS_CTRLVOLUME ;
	mDXSoundBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	mDXSoundBufferDesc.lpwfxFormat = mWaveformObj.GetWaveform();

	mDXSoundObj->CreateSoundBuffer(&mDXSoundBufferDesc, &mDXSoundBuffer, NULL);

	//Copy Waveform Data to Sound Buffer
	void *tempDXSoundBuffer;
	DWORD numBytesBuffer1, numBytesBuffer2;
	
	mDXSoundBuffer->Lock(NULL, NULL, &tempDXSoundBuffer, &numBytesBuffer1, NULL, &numBytesBuffer2, DSBLOCK_ENTIREBUFFER);
	memcpy(tempDXSoundBuffer, mWaveformObj.GetWaveformData(), mWaveformObj.GetWaveformLength());
	mDXSoundBuffer->Unlock(tempDXSoundBuffer, numBytesBuffer1, NULL, numBytesBuffer2);
}

//Set Sound ID
void DXSound::SetDXSoundID(int newID)
{
	mDXSoundID = newID;
}

//Return Sound ID
int DXSound::GetDXSoundID()
{
	return mDXSoundID;
}

void DXSound::CleanDXSound()
{
	if(mDXSoundObj)
	{
		mDXSoundObj->Release();
		mDXSoundObj = NULL;
	}

	if(mDXSoundBuffer)
	{
		mDXSoundBuffer->Release();
		mDXSoundBuffer = NULL;
	}
}

LPDIRECTSOUNDBUFFER DXSound::GetDXSoundBuffer()
{
	return mDXSoundBuffer;
}