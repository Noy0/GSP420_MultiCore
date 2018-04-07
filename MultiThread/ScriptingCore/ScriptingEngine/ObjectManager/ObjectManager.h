#pragma once
#include "..\\ScriptUtil.h"
#include "..\\ScriptNodes\\BaseNode.h"

typedef map<EEvent, CBaseNode*> Events;

typedef struct SObjectInfo{

	CBaseNode* ObjectScript;
	Events ObjEvents;

	SObjectInfo(){

		ObjectScript = NULL;
	}
}OBJECT_INFO;

class CObjectManager{

public:
	CObjectManager();
	~CObjectManager();

	void AddObject(string ObjName, CBaseNode* newObjScript);
	void AddObjectEvent( string ObjName, EEvent newEvent, CBaseNode* startEvent );

	void RunObjectScript( string objName );
	void RunObjectEvent( string objName, EEvent objEvent );

	CBaseNode* GetObjectScript( string obj );

	bool IsType( string type );
	bool HasEvent( string type, EEvent e );

private:
	map<string, OBJECT_INFO> mObjMap;
};