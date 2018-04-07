#include "ScriptingCore.h"
#include "ScriptMessages.h"
#include "..\\CoreManager.h"

CScriptingCore::CScriptingCore(){

	mSEngine = new CScriptingEngine();
	mSData = 0;
	mSEngine->Compile();
}

CScriptingCore::~CScriptingCore(){

	map<int, pair<CBaseNode*, FLAGS>>::iterator it = mObjFlags.begin();
	while( !mObjFlags.empty() ){

		delete it->second.first;
		it->second.first = 0;

		mObjFlags.erase( it );

		it = mObjFlags.begin();
	}
	delete mSEngine;
	mSEngine = 0;
}

void CScriptingCore::Update(float dt){

	CopyMessage();
	HandleScriptMsg();

	if(mSData != 0)
	{
		vector<ScriptData> tempData;

		mSData->Copy(tempData);

		//do stuff with the data
		for( unsigned int i =  0;i < tempData.size();i++ ){

			int currID = tempData[i].objID;
			string currType = tempData[i].objType;
			FuncMgr->SetCurrentID( currID );

			if( mObjFlags.find( currID ) == mObjFlags.end() ){

				FLAGS temp;

				mObjFlags[ currID ].second = temp;

				//mObjFlags[ currID ].first = CopyScript( mSEngine->GetObjectScript( currType ) );
			}

			if( tempData[i].LMouseClick != mObjFlags[ currID ].second.LMouseClick && mSEngine->HasEvent( currType, LEFT_CLICK ) ){

				mObjFlags[ currID ].second.LMouseClick = tempData[i].LMouseClick;

				mSEngine->RunObjectEvent( currType, LEFT_CLICK );
			}

			if( tempData[i].RMouseClick != mObjFlags[ currID ].second.RMouseClick && mSEngine->HasEvent( currType, RIGHT_CLICK ) ){

				mObjFlags[ currID ].second.RMouseClick = tempData[i].RMouseClick;

				mSEngine->RunObjectEvent( currType, RIGHT_CLICK );
			}

			//CBaseNode* temp = mObjFlags[ currID ].first;

			//do{

			//	temp = temp->Execute();
			//}while( temp != NULL );
			
		}
	}
//	mSEngine->UpdateAllScripts( dt );
}

void CScriptingCore::OnLauch(){

	//gCoreMgr->HandleMessage( new SMessageRunScript( "test.txt" ) );
}

void CScriptingCore::HandleScriptMsg(){

	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
			case MSG_RUN_SCRIPT:
				//SMessageRunScript* nmsg = (SMessageRunScript*)msg;
				//mSEngine->RunScript( nmsg->script );
				mSEngine->RunScript(((SMessageRunScript*)msg)->script);
				delete msg;
				break;
			case MSG_REG_SDATA:
				//SMessageRegisterScriptObject* nmsg = (SMessageRegisterScriptObject*)msg;
				mSData = ((SMessageRegisterScriptObject*)msg)->SData;
				delete msg;
				break;
		}

		m_QueueBuffer.pop();
	}
}

CBaseNode* CScriptingCore::CopyScript(CBaseNode *objScript){

	CBaseNode* temp = objScript;
	CBaseNode* copy = NULL;
	CBaseNode* currNode = NULL;

	do{

		switch(temp->NType){

			case NTYPE_VAR_CREATE:
				currNode = new VAR_CREATE();
				reinterpret_cast<VAR_CREATE*>(currNode)->expression = reinterpret_cast<VAR_CREATE*>(temp)->expression;
				reinterpret_cast<VAR_CREATE*>(currNode)->varType = reinterpret_cast<VAR_CREATE*>(temp)->varType;
				reinterpret_cast<VAR_CREATE*>(currNode)->varName = reinterpret_cast<VAR_CREATE*>(temp)->varName;
				reinterpret_cast<VAR_CREATE*>(currNode)->Owner = reinterpret_cast<VAR_CREATE*>(temp)->Owner;
				break;

			case NTYPE_VAR_ASSIGNMENT:
				currNode = new VAR_ASSIGNMENT();
				reinterpret_cast<VAR_ASSIGNMENT*>(currNode)->expression = reinterpret_cast<VAR_ASSIGNMENT*>(temp)->expression;
				reinterpret_cast<VAR_ASSIGNMENT*>(currNode)->varName = reinterpret_cast<VAR_ASSIGNMENT*>(temp)->varName;
				reinterpret_cast<VAR_ASSIGNMENT*>(currNode)->Owner = reinterpret_cast<VAR_ASSIGNMENT*>(temp)->Owner;
				break;

			case NTYPE_FUNCTION_CALL:
				currNode = new FUNCTION_CALL();
				reinterpret_cast<FUNCTION_CALL*>(currNode)->args = reinterpret_cast<FUNCTION_CALL*>(temp)->args;
				reinterpret_cast<FUNCTION_CALL*>(currNode)->Function = reinterpret_cast<FUNCTION_CALL*>(temp)->Function;
				reinterpret_cast<FUNCTION_CALL*>(currNode)->Owner = reinterpret_cast<FUNCTION_CALL*>(temp)->Owner;
				break;

			case NTYPE_IF_BEGIN:
				currNode = new IF_BEGIN();
				reinterpret_cast<IF_BEGIN*>(currNode)->condition = reinterpret_cast<IF_BEGIN*>(temp)->condition;
				reinterpret_cast<IF_BEGIN*>(currNode)->ifElse = reinterpret_cast<IF_BEGIN*>(temp)->ifElse;
				reinterpret_cast<IF_BEGIN*>(currNode)->ifEnd = reinterpret_cast<IF_BEGIN*>(temp)->ifEnd;
				reinterpret_cast<IF_BEGIN*>(currNode)->Owner = reinterpret_cast<IF_BEGIN*>(temp)->Owner;
				break;

			case NTYPE_IF_END:
				currNode = new IF_END();
				break;

			case NTYPE_WHILE_BEGIN:
				currNode = new WHILE_BEGIN();
				reinterpret_cast<WHILE_BEGIN*>(currNode)->condition = reinterpret_cast<WHILE_BEGIN*>(temp)->condition;
				reinterpret_cast<WHILE_BEGIN*>(currNode)->whileEnd = reinterpret_cast<WHILE_BEGIN*>(temp)->whileEnd;
				reinterpret_cast<WHILE_BEGIN*>(currNode)->Owner = reinterpret_cast<WHILE_BEGIN*>(temp)->Owner;
				break;

			case NTYPE_WHILE_END:
				currNode = new WHILE_END();
				reinterpret_cast<WHILE_END*>(currNode)->whileBegin = reinterpret_cast<WHILE_END*>(temp)->whileBegin;
				break;

			case NTYPE_ELSE:
				currNode = new ELSE();
				reinterpret_cast<ELSE*>(currNode)->ifEnd = reinterpret_cast<ELSE*>(temp)->ifEnd;
				reinterpret_cast<ELSE*>(currNode)->Owner = reinterpret_cast<ELSE*>(temp)->Owner;
				break;
		}

		CBaseNode* b = copy;

		if( b == NULL ){

			copy = currNode;
		}
		else{

			while( b->next != NULL ){

				b = b->next;
			}

			b->next = currNode;
		}
		currNode = NULL;

		temp = temp->next;

	}while(temp != NULL);

	return copy;
}