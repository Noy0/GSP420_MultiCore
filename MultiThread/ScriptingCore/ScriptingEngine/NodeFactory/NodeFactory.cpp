#include "NodeFactory.h"

CNodeFactory::CNodeFactory(){

	mListStart = NULL;
}

CNodeFactory::~CNodeFactory(){

	//if( mListStart ){

	//	delete mListStart;
	//	mListStart = 0;
	//}
}

void CNodeFactory::BeginNewList( string newOwner ){

	mListStart    = NULL;
	mCurrentOwner = newOwner;
}

void CNodeFactory::AddVariableNode(string type, string varName,string exp,int scope, int line)
{
	VAR_CREATE* newVar = new VAR_CREATE();

	newVar->Owner = mCurrentOwner;

	newVar->varName = varName;

	newVar->NType =	NTYPE_VAR_CREATE;
	
	newVar->expression = exp;

	newVar->varType = type;

	newVar->scope = scope;

	newVar->lineNumber = line;

	AddToList(newVar);
}

void CNodeFactory::AddAssignmentNode( string varName, string exp, int line ){

	VAR_ASSIGNMENT* newAssign = new VAR_ASSIGNMENT();

	newAssign->Owner = mCurrentOwner;

	newAssign->varName = varName;

	newAssign->NType = NTYPE_VAR_ASSIGNMENT;

	newAssign->expression = exp;

	newAssign->lineNumber = line;

	AddToList( newAssign );
}

void CNodeFactory::AddConditionalNode( string newCondition, int type, int line ){

	if( type == 0 ){

		IF_BEGIN* newif = new IF_BEGIN();

		newif->Owner = mCurrentOwner;

		newif->condition =  newCondition;

		newif->NType = NTYPE_IF_BEGIN;

		newif->ifEnd = NULL;
		newif->ifElse = NULL;

		newif->lineNumber = line;

		mIfStack.push( newif );

		AddToList( newif );
	}
	else if( type == 1 ){
		
		WHILE_BEGIN* newWhile = new WHILE_BEGIN();

		newWhile->Owner = mCurrentOwner;

		newWhile->condition = newCondition;

		newWhile->whileEnd = NULL;

		newWhile->NType = NTYPE_WHILE_BEGIN;

		newWhile->lineNumber = line;

		mWhileStack.push( newWhile );

		AddToList( newWhile );
	}
}

void CNodeFactory::AddFunctionNode( FUNC function, vector<string> newArgs, int line ){

	FUNCTION_CALL* newFunc = new FUNCTION_CALL();

	newFunc->Owner = mCurrentOwner;

	newFunc->Function = function;

	newFunc->NType = NTYPE_FUNCTION_CALL;

	newFunc->args = newArgs;

	newFunc->lineNumber = line;

	AddToList( newFunc );
}

void CNodeFactory::AddIfEnd(int line){

	IF_BEGIN* tempif = mIfStack.top();
	IF_END* ifend    = new IF_END();

	ifend->Owner = mCurrentOwner;

	ifend->NType = NTYPE_IF_END;

	ifend->lineNumber = line;

	tempif->ifEnd = ifend;

	if( tempif->ifElse != NULL ){

		tempif->ifElse->ifEnd = ifend;
	}

	mIfStack.pop();

	AddToList( ifend );

}

void CNodeFactory::AddWhileEnd(int line){

	WHILE_BEGIN* tempw = mWhileStack.top();
	WHILE_END* wend    = new WHILE_END();

	wend->Owner = mCurrentOwner;

	wend->lineNumber = line;

	wend->NType = NTYPE_WHILE_END;

	tempw->whileEnd = wend;

	wend->whileBegin = tempw;
	mWhileStack.pop();

	AddToList( wend );
}

void CNodeFactory::AddElseNode(int line){

	IF_BEGIN* tempif = mIfStack.top();
	ELSE* ifelse    = new ELSE();

	ifelse->Owner = mCurrentOwner;

	ifelse->lineNumber = line;

	ifelse->NType = NTYPE_ELSE;

	tempif->ifElse = ifelse;

	AddToList( ifelse );
}

void CNodeFactory::AddToList( CBaseNode* newNode ){

	CBaseNode* temp = mListStart;

	if( temp == NULL ){

		mListStart = newNode;
	}
	else{

		while( temp->next != NULL ){

			temp = temp->next;
		}

		temp->next = newNode;
	}
}

CBaseNode* CNodeFactory::GetCurrentListStart(){

	return mListStart;
}