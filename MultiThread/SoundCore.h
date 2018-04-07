#pragma once
#include "BaseCore.h"
#include "Sound\\SoundEngine.h"

class SoundCore:public BaseCore
{
public:
	SoundCore(HWND handle);
	~SoundCore();

	void Update( float dt );

	void HandleSoundMessage();

private:
	SoundEngine mSoundEng;

};