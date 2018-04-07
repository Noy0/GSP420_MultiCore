#include "EventManager.h"

CEventManager::CEventManager(){
}

CEventManager::~CEventManager(){

	map<string, CBaseNode*>::iterator it = mEventMap.begin();
	for( ;it != mEventMap.end();++it ){

		if( it->second ){

			delete it->second;
			it->second = 0;
		}
	}

	mEventMap.clear();
}

void CEventManager::RunEvent( string eventName ){

	CBaseNode* temp = mEventMap.find( eventName )->second;

	do{

		temp = temp->Execute();

	}while( temp != NULL );
}

void CEventManager::AddEvent( string eventName, CBaseNode* newEvent ){

	mEventMap[ eventName ] = newEvent;
}