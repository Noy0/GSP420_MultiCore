#include "DXSoundMgr.h"
//#include <d3dx9.h>

DXSoundMgr::DXSoundMgr(HWND handle)
{
	//Set initial sound amount
	mNumDXSound = 0;
	//LoadSounds();
	mHandle = handle;
}

//DXSoundMgr* DXSoundMgr::Instance()
//{
//	static DXSoundMgr instance;
//	return &instance;
//}

//Adds a sound to the manager
void DXSoundMgr::AddDXSound(DXSound *pDXSoundObj)
{
	mNumDXSound++;
	pDXSoundObj->SetDXSoundID(mNumDXSound);
	mDXSoundVec.push_back(pDXSoundObj);
}

//Releases all sounds in the manager
void DXSoundMgr::CleanAllDXSound()
{
	for(int i = 0; i < mNumDXSound; i++)
	{
		mDXSoundVec.at(i)->CleanDXSound();
	}
}

//Loads all sounds
void DXSoundMgr::LoadSounds()
{
	inFile.open("Sound//SoundsToLoad.txt");

	if(inFile.is_open())
	{
		while(!inFile.eof())
		{
			std::getline(inFile, mCurrLine);

			DXSound* s = new DXSound();
			sprintf_s(buffer, "%s", mCurrLine.c_str());
			s->LoadDXSound(buffer, mHandle);
			mDXSoundVec.push_back(s);
		}
		inFile.close();
	}
	//else
	//{
	//	//MessageBox(gDXBase->getMainWnd(), "Failed to load: SoundsToLoad.txt", "Error", MB_OK);
	//}
}

//Returns the vector of sounds
std::vector<DXSound*>* DXSoundMgr::GetDXSoundVec()
{
	return &mDXSoundVec;
}

//Returns an element within the sound vector
DXSound* DXSoundMgr::GetDXSound(int index)
{
	return mDXSoundVec[index];
}