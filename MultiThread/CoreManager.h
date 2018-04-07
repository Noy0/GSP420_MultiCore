#ifndef _COREMANAGER_H
#define _COREMANAGER_H

#include "BaseCore.h"
#include "Message.h"
#include <vector>
using std::vector;

//CoreManager class
//Singleton class, Initialized using Create() function
//Stores map to all cores and dispatches messages.

enum ECore;

class CoreManager
{
public:
	static void Create();
	void AddCore(BaseCore* pCore, ECore nID);
	void RemoveCore(ECore nID);
	void SetCoreFramesPerSecond(ECore CoreID, float fps);
	float GetLastSPF(ECore nID);
	void Launch();
	void Update(float dt);
	bool HandleMessage(SMessage* pMessage);
	void Terminate(ECore CoreID);
	void TerminateAll();
	bool AllStopped();
	~CoreManager();
private:
	//Private constructor
	CoreManager();
	//Array of pointers to cores
	vector<BaseCore*> m_vCoreMap;

	bool m_Launched;
};

//Singleton is stored in this global pointer.
extern CoreManager* gCoreMgr;

#endif