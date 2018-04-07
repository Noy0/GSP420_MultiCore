#ifndef DXSoundMgr_H
#define DXSoundMgr_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "DXSound.h"
//#include "DXBase.h"

class DXSoundMgr
{
public:
	//static DXSoundMgr* Instance();
	DXSoundMgr(HWND handle);
	//Adds a sound pointer to the manager
	void AddDXSound(DXSound* pDXSoundObj);
	//Free all sounds from te manager
	void CleanAllDXSound();
	//Load all sounds from file
	void LoadSounds();

	DXSound* GetDXSound(int index);

	//Returns the vector of sounds
	std::vector<DXSound*>* GetDXSoundVec();
private:
	

	std::ifstream inFile;
	std::string mCurrLine;
	char buffer[128];

	int mNumDXSound;
	std::vector<DXSound*> mDXSoundVec;
	HWND mHandle;
};

#endif