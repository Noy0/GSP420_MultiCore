#pragma once
#include "..\\ScriptUtil.h"
#include "..\\ScriptNodes\\ScriptNodes.h"

class CNodeFactory{

public:
	CNodeFactory();
	~CNodeFactory();

	void BeginNewList( string newOwner );
	void AddVariableNode(string type, string varName, string exp,int scope, int line);
	void AddAssignmentNode( string varName, string exp, int line );
	void AddConditionalNode( string newCondition, int type, int line );
	void AddFunctionNode( FUNC function, vector<string> newArgs, int line );
	void AddIfEnd(int line);
	void AddWhileEnd(int line);
	void AddToList( CBaseNode* newNode );
	void AddElseNode(int line);

	CBaseNode* GetCurrentListStart();

private:
	CBaseNode* mListStart;
	string mCurrentOwner;
	stack<CIfBeginNode*> mIfStack;
	stack<CWhileBeginNode*> mWhileStack;
};