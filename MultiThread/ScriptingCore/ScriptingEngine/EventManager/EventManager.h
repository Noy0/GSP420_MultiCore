#pragma once
#include "..\\ScriptUtil.h"
#include "..\\ScriptNodes\\BaseNode.h"

class CEventManager{

public:
	CEventManager();
	~CEventManager();

	void RunEvent( string eventName );
	void AddEvent( string eventName, CBaseNode* eventStart );

private:
	map<string, CBaseNode*> mEventMap;
	
};

extern CEventManager EvntMgr;