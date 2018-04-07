#pragma once
#include <windows.h>
//======debug======
#include <iostream>
using std::cout;
using std::endl;
//=================

#include "ScriptNodes\\ScriptNodes.h"
#include "FunctionManager\\FunctionManager.h"
#include "NodeFactory\\NodeFactory.h"
#include "EventManager\\EventManager.h"
#include "VariableManager\\VariableManager.h"
#include "ObjectManager\\ObjectManager.h"
#include "ScriptUtil.h"
#include "Debugger\\Debug.h"

class CScriptingEngine{

public:
	CScriptingEngine();
	~CScriptingEngine();

	void RunScript( string scriptName );
	void _RunEvent( string eventName );
	void RemoveLeadingWhiteSpace(string* str);
	void RemoveWhiteSpace(string* str);
	void Compile();
	void ReCompile();
	bool CheckExp(string exp,Error& errorType);
	bool CountParens(string exp);
	bool CheckVars(string str);
	void Debug();
	void RunObjectScript( string type );
	void RunObjectEvent( string type, EEvent e );

	CBaseNode* GetObjectScript( string obj );

	bool IsType( string type );
	bool HasEvent( string type, EEvent e );

	void UpdateAllScripts( float dt );
    bool GoodBuild;  //true if error free

private:

	CEventManager    mEventMgr;

	CNodeFactory     mNFactory;
	
	map<string, CBaseNode*> mScriptMap;

	WIN32_FIND_DATA mScriptData;
	HANDLE mScriptHandle;

	
	vector<string> VarList;  //holds list of variable names

	CDebug mCompileLog;

	CObjectManager mObjMgr;

	ifstream mLoadFile;
};