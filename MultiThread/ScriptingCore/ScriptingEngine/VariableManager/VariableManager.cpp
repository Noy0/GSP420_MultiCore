#include "VariableManager.h"

CVariableManager* CVariableManager::_instance = 0;
CVariableManager::CVariableManager(){
}

CVariableManager::~CVariableManager(){

	//Destroy all Global Variables
	for( unsigned int i = 0;i < mGlobalVariables.size();i++ ){

		DestroyVariable( mGlobalVariables[i] );
	}
	
	//Destroy all local variables for every script
	map<string, vector<VARIABLE*>>::iterator it = mLocalVariables.begin();
	for( ;it != mLocalVariables.end();++it ){

		for( unsigned int i = 0;i < it->second.size();i++ ){

			DestroyVariable( it->second[i] );
		}
	}
	
	//if the stack still has something in it then clean it
	for( unsigned int i = 0;i < mStack.size();i++ ){

		if( mStack[i] ){

			delete mStack[i];
			mStack[i] = 0;
		}
	}

	mGlobalVariables.clear();
	mLocalVariables.clear();
	mStack.clear();
}

CVariableManager* CVariableManager::Instance(){

	if( _instance == 0 )
		_instance = new CVariableManager;

	return _instance;
}
//Safely destroy a VARIABLE 
void CVariableManager::DestroyVariable( VARIABLE* dVar ){

	
	if( dVar->second.second && dVar->second.first != "int"){

		delete dVar->second.second;
		dVar->second.second = 0;
	}

	if( dVar ){

		delete dVar;
		dVar = 0;
	}
}

void CVariableManager::CleanUp(){

	//Destroy all Global Variables
	for( unsigned int i = 0;i < mGlobalVariables.size();i++ ){

		DestroyVariable( mGlobalVariables[i] );
	}
	
	//Destroy all local variables for every script
	map<string, vector<VARIABLE*>>::iterator it = mLocalVariables.begin();
	for( ;it != mLocalVariables.end();++it ){

		for( unsigned int i = 0;i < it->second.size();i++ ){

			DestroyVariable( it->second[i] );
			
			it->second[i] = 0;
		}
	}
	
	//if the stack still has something in it then clean it
	for( unsigned int i = 0;i < mStack.size();i++ ){

		if( mStack[i] ){

			delete mStack[i];
			mStack[i] = 0;
		}
	}

	mGlobalVariables.clear();
	mLocalVariables.clear();
	mStack.clear();


}

//Register variables so that that are saved in their proper scope and script
void CVariableManager::RegisterVariable( string scriptName, VARIABLE* newVariable, int scope ){

	if( scope == 0 ){

		mLocalVariables[ scriptName ].push_back( newVariable );
	}
	else if( scope == 1 ){

		mGlobalVariables.push_back( newVariable );
	}
}


void CVariableManager::AssignVariable(string scriptName, string varName, VARIABLE* newVariable)
{
	VARIABLE* temp = GetVarByName(scriptName,varName);

	if(temp != NULL)
	{
		temp = newVariable;
	}
	

}


void CVariableManager::PushToStack(int Variable)
{
	mStack.push_back(new int( Variable ) );
}

void CVariableManager::PushToStack(float Variable)
{
	mStack.push_back(new float(Variable));
}

void CVariableManager::PushToStack(string Variable)
{
	mStack.push_back(new string(Variable));
}

//Adds values to the stack so that they can be retrieved in weapper functions
void CVariableManager::PushToStack( VARIABLE* Variable ){

	mStack.push_back( Variable->second.second );
}

void CVariableManager::Destroy(){

	delete _instance;
	_instance = 0;
}

//returns the index of the stack as a float
float CVariableManager::StackToFloat( int index ){

	return *( (float* ) mStack[index] );
}

//returns the index of the stack as a int
int CVariableManager::StackToInt( int index ){

	return *( ( int* ) mStack[index] );
}

//returns the index of the stack as a string
string CVariableManager::StackToString( int index ){

	return *( ( string* ) mStack[index] );
}

//return a VARIABLE by name, and what script it is in, or if it is global
VARIABLE* CVariableManager::GetVarByName( string scriptName, string variable){

	if( mLocalVariables.size() != 0 ){

		for( unsigned int i = 0;i < mLocalVariables[ scriptName ].size();i++ ){

			if( mLocalVariables[ scriptName ][i]->first == variable ){

				return mLocalVariables.find( scriptName )->second.at( i );
			}
		}
	}

	if( mGlobalVariables.size() != 0 ){

		for( unsigned int i = 0;i < mGlobalVariables.size();i++ ){

			if( mGlobalVariables[i]->first == variable ){

				return mGlobalVariables[i];
			}
		}
	}

	return NULL;
}

//creates a new string variable VARIABLE
VARIABLE* CVariableManager::MakeStringVar( string varName, string value ){

	VARIABLE* temp = new VARIABLE();

	temp->first = varName;
	temp->second.first = "string";
	temp->second.second = new string (value);

	return temp;
}

VARIABLE* CVariableManager::MakeIntVar( string varName, int value ){

	VARIABLE* temp = new VARIABLE();

	temp->first = varName;
	temp->second.first = "int";
	temp->second.second = (void*)(value);

	return temp;
}

VARIABLE* CVariableManager::MakeFloatVar( string varName, float value ){

	VARIABLE* temp = new VARIABLE();

	temp->first = varName;
	temp->second.first = "float";
	temp->second.second = new float (value);

	return temp;
}

void CVariableManager::ClearStack()
{
	for( unsigned int i = 0;i < mStack.size();i++ ){

		if( mStack[i] ){

			delete mStack[i];
			mStack[i] = NULL;
		}
	}

	mStack.clear();
}