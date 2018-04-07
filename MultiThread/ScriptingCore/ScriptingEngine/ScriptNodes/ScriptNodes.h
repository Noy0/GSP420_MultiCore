#pragma once
//===============================================================
// ScriptNodes.h
// ScriptNodes are used to build a link lists of script commands.
// the list  is built when Compile() is called from
// the ScriptingEngine
//===============================================================

#include "BaseNode.h"

class CWhileBeginNode;


typedef class CVariableNode : public CBaseNode{
	
public:
	CVariableNode();
	~CVariableNode();

	virtual CBaseNode* Execute();

	string varName;

	string expression;

	string varType;

	bool created;

	int scope;



	
}VAR_CREATE;


//============================================
// class CAssignmentNode
// this node is used to assign a value into a
// variable, value is determined from:
// variable name, function call, or hard value
//============================================
typedef class CAssignmentNode : public CBaseNode{
	
public:
	CAssignmentNode();
	~CAssignmentNode();

	virtual CBaseNode* Execute();

	//VARIABLE* assignment;

	//VARIABLE* value;

	string varName;

	//float* floatVar;

	string expression;
	
}VAR_ASSIGNMENT;

//===============================================
// class CFunctionNode
// When executed this node will load all of the
// the values from the args vector into the stack.
// then it will call the function pointer
// ===============================================
typedef class CFunctionNode : public CBaseNode{

public:
	CFunctionNode();
	~CFunctionNode();

	virtual CBaseNode* Execute();

	vector<string> args;

	int (*Function)();
}FUNCTION_CALL;

//===============================================
// class CIfEndNode
// this is used only for the CIfBeginNode to have
// something to go to if its condition is false
//===============================================

typedef class CIfEndNode : public CBaseNode{

public:
	CIfEndNode(){}
	~CIfEndNode(){}

	virtual CBaseNode* Execute();
}IF_END;

//================================
// class CElseNode
//================================
typedef class CElseNode : public CBaseNode{

public:
	CElseNode();
	~CElseNode();

	bool didIf;

	CIfEndNode* ifEnd;

	virtual CBaseNode* Execute();
}ELSE;
//=================================================
// class CIfBeginNode
// This is the beginning of an if statement,
// it uses the condition that is passed to it
// when it is created to determine wheter or not it
// should return the next or ifEnd->next
//=================================================
typedef class CIfBeginNode : public CBaseNode{

public:
	CIfBeginNode(){}
	~CIfBeginNode();

	bool Conditional( string condition );

	virtual CBaseNode* Execute();

	string condition;

	CIfEndNode* ifEnd;
	CElseNode* ifElse;
}IF_BEGIN;

//================================================
// class CWhileEndNode
// When executed this node will return the pointer
// to its coresponding CWhileBeginNode
//================================================
typedef class CWhileEndNode : public CBaseNode{

public:
	CWhileEndNode(){}
	~CWhileEndNode();

	virtual CBaseNode* Execute();

	CWhileBeginNode* whileBegin;
}WHILE_END;

//===============================================
// class CWhileBeginNode
// When this node is executed it will first check
// its condition, if false then it will return
// whileEnd->next, else it will just return next
//===============================================
typedef class CWhileBeginNode : public CBaseNode{

public:
	CWhileBeginNode(){}
	~CWhileBeginNode();

	bool Conditional( string condition );

	virtual CBaseNode* Execute();

	string condition;

	CWhileEndNode* whileEnd;
}WHILE_BEGIN;

typedef class CSleepNodeNode : public CBaseNode{

public:
	float timeSleeping;

	virtual CBaseNode* Execute();

	CSleepNodeNode(){timeSleeping = 0.0f;}
	~CSleepNodeNode();
}SLEEP;