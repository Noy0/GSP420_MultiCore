#ifndef VARIABLEMANAGER_H
#define VARIABLEMANAGER_H
#pragma once
#include "..\\ScriptUtil.h"
//==============================
// class CVariableManager
// 
class CVariableManager{

public:
	CVariableManager();
	~CVariableManager();

	static CVariableManager* Instance();

	void RegisterVariable( string scriptName, VARIABLE* newVariable, int scope );
	void AssignVariable(string scriptName, string varName, VARIABLE* newVariable);
	void PushToStack( VARIABLE* Variable );
	void PushToStack( int Variable );
	void PushToStack( float Variable );
	void PushToStack( string Varible );
	void DestroyVariable( VARIABLE* dVar );
	void ClearStack();
	void Destroy();
	void CleanUp();

	float StackToFloat( int index );
	int StackToInt( int index );
	string StackToString( int index );

	VARIABLE* GetVarByName( string scriptName, string variable );
	VARIABLE* MakeStringVar( string varName, string value );
	VARIABLE* MakeIntVar( string varName, int value );
	VARIABLE* MakeFloatVar( string varName, float value );

private:
	vector<VARIABLE*> mGlobalVariables;
	map<string, vector<VARIABLE*>> mLocalVariables;
	vector<void*> mStack;

	static CVariableManager* _instance;
};

#define VarMgr CVariableManager::Instance()

#endif