#ifndef SoundCore_H
#define SoundCore_H
#include "DXSoundMgr.h"

class SoundEngine
{
public:
	//static SoundCore* Instance();
	SoundEngine(HWND handle);
	~SoundEngine();
	//plays the song at the index of the vector
	void Play(int song, bool loop);
	//stops the song at the index of the vector
	void Stop(int song);
	//sets volume of the indexd song to a value of the indexd attunation
	void SetVolume(int song,int att);
	//loads all sounds from a text file
	void LoadSound();
	//clears all sounds from memory
	void CleanSound();
	

private:
	// float array that holds all attunations we will be using
	float Volume[11];
	DXSoundMgr soundMgr;
	
	
};

#endif