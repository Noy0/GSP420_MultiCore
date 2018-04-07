#include "ObjectManager.h"

CObjectManager::CObjectManager(){
}

CObjectManager::~CObjectManager(){

	map<string, OBJECT_INFO>::iterator it = mObjMap.begin();
	while( !mObjMap.empty() ){

		delete it->second.ObjectScript;
		it->second.ObjectScript = 0;

		map<EEvent, CBaseNode*>::iterator nit = it->second.ObjEvents.begin();
		while(!it->second.ObjEvents.empty())
		{
			delete nit->second;
			nit->second = 0;

			it->second.ObjEvents.erase(nit);

			nit = it->second.ObjEvents.begin();
		}
		mObjMap.erase( it );

		it = mObjMap.begin();
	}


	mObjMap.clear();
}

void CObjectManager::AddObject( string ObjName, CBaseNode *newObjScript ){

	OBJECT_INFO temp;

	temp.ObjectScript = newObjScript;

	mObjMap[ ObjName ] = temp;
}

void CObjectManager::AddObjectEvent( string ObjName, EEvent newEvent, CBaseNode *startEvent ){

	mObjMap[ ObjName ].ObjEvents[ newEvent ] = startEvent;
}

void CObjectManager::RunObjectScript( string objName ){

	if( mObjMap.find( objName ) != mObjMap.end() ){

		CBaseNode* temp = mObjMap.find( objName )->second.ObjectScript;

		do{

			temp = temp->Execute();

		}while( temp != NULL );
	}
}

void CObjectManager::RunObjectEvent( string objName, EEvent objEvent ){

	if( mObjMap[ objName ].ObjEvents.find( objEvent ) != mObjMap[ objName ].ObjEvents.end() ){


		CBaseNode* temp = mObjMap[ objName ].ObjEvents.find( objEvent )->second;

		do{

			temp = temp->Execute();

		}while( temp != NULL );
	}
}

bool CObjectManager::IsType( string type ){

	if( mObjMap.find( type ) == mObjMap.end() )
		return false;

	return true;
}

bool CObjectManager::HasEvent( string type, EEvent e ){

	if( mObjMap[ type ].ObjEvents.find( e ) == mObjMap[ type ].ObjEvents.end() )
		return false;

	return true;
}

CBaseNode* CObjectManager::GetObjectScript( string obj ){

	if( mObjMap.find( obj ) == mObjMap.end() )
		return NULL;

	return mObjMap[ obj ].ObjectScript;
}
