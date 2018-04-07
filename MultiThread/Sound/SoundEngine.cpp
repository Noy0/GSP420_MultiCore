#include "SoundEngine.h"


SoundEngine::SoundEngine(HWND handle):soundMgr(handle)
{
//sets the attiunation of the volume levels  by every 10%
	Volume[0]= -10000;//muted
	Volume[1]= -1000 ;//10%
	Volume[2]= -698.97000433602;//20%
	Volume[3]= -522.87874528034;//30%
	Volume[4]= -397.94000867204;//40%
	Volume[5]= -301.02999566398;//50%
	Volume[6]= -221.84874961636;//60%
	Volume[7]= -154.90195998574;//70%
	Volume[8]= -96.910013008056;//80%
	Volume[9]= -45.757490560675;//90%
	Volume[10]= 0;// Full volume (no attunation)
	LoadSound();
}

SoundEngine::~SoundEngine()
{
	CleanSound();
}
//SoundCore* SoundCore::Instance()
//{
//	static SoundCore instance;
//	return &instance;
//}

void SoundEngine::Play(int song, bool loop)
{
	// plays the song sent to the funtion
	if(loop == true)
	{
		soundMgr.GetDXSound(song)->GetDXSoundBuffer()->Play(NULL, NULL, DSBPLAY_LOOPING);
		
	}
	else
	{
		soundMgr.GetDXSound(song)->GetDXSoundBuffer()->Play(NULL, NULL, 0);
	}
}

void SoundEngine::Stop(int song)
{
	soundMgr.GetDXSound(song)->GetDXSoundBuffer()->Stop();
}

void SoundEngine::SetVolume(int song, int att)
{
	soundMgr.GetDXSound(song)->GetDXSoundBuffer()->SetVolume(Volume[att]);
}

void SoundEngine::LoadSound()
{
	soundMgr.LoadSounds();
}

void SoundEngine::CleanSound()
{
	soundMgr.CleanAllDXSound();
}

