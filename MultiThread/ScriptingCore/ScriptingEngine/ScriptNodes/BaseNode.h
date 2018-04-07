#pragma once
#include "..\\ScriptUtil.h"
#include "..\\VariableManager\\VariableManager.h"
#include "..\\FunctionManager\\FunctionManager.h"
//===========================================
//Class CBaseNode
// This is the base for all nodes to be created for the
// linklist in the CNodeFactory has a pointer to the next
// node in the list. Has virtual Execute function so that
// all nodes and execute themselves, it also return a pointer
// to the next node to be executed
//===========================================
class CBaseNode{

public:

	virtual CBaseNode* Execute() = 0;

	CBaseNode* next;
	int lineNumber;

	string Owner;

	ENodeType NType;

	int GetDelimiter(string str, int& pos);

	template <class Type>
	Type EvalExp(string script,string exp);

	int ConditionDelimiter(string str, int& pos);

	int MatchParentheses(string str);

	bool InParentheses(string str, int pos);

	bool SearchForOperator(string str, char ch, int& pos);

	float dt;

	CBaseNode()
	{
		next = NULL;
	}
	virtual ~CBaseNode(){

		if( next ){

			delete next;
			next = 0;
		}
	}
};