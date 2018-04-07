#pragma once

#include "..\\BaseCore.h"
#include "ScriptingEngine\\ScriptingEngine.h"
#include "..\\DDScriptData.h"

using std::map;

class CScriptingCore : public BaseCore{

public:
	CScriptingCore();
	~CScriptingCore();

	void Update( float dt );

	void OnLauch();

	void HandleScriptMsg();

	CBaseNode* CopyScript( CBaseNode* objScript );

private:
	CScriptingEngine* mSEngine;
	map<int, pair<CBaseNode*, FLAGS>> mObjFlags;

	DD_ScriptData* mSData;

	//void HandleScriptMessages();
};
