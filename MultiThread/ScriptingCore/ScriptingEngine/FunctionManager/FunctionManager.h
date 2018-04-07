#pragma once
#include "..\\ScriptUtil.h"
#include "..\\VariableManager\\VariableManager.h"
//======Debug======
#include <iostream>
using std::cout;
using std::endl;
//=================
//===================================================
// class CFunctionManager
// This class holds all pointers to wrapper functions
// that will be called from a CFunctionNode. The 
// mWrappers will be filled with function names and 
// function pointer to those functions
//===================================================

class CFunctionManager{

public:
	CFunctionManager();
	~CFunctionManager();

	static CFunctionManager* Instance();

	FUNC GetFunction( string funcName );
	bool FindFunction( string funcName); //new
	void Destroy();
	void SetCurrentID( int newID );

	int* CurrentID(){ return &mCurrentID;}

private:
	map<string, FUNC> mWrappers;

	static CFunctionManager* _instance;

	int mCurrentID;

};

#define FuncMgr CFunctionManager::Instance()
//=======================================================
// Fill this namespace in with all of the wrapper functions
// that will wrap up all the functions of everyone else's
// functions
//====================WRAPPERS===========================
namespace Wrappers{

	int HelloWorld();
	int TypeYourName();
	int PrintValue();	//Calls Print
	int _Add2Ints();

	//Networking wrappers
	int SwitchToServer();
	int SwitchToClient();
	int SendData();
	int GetLastMessage();
	//int GetMessage();

	//UI wrappers
	int AddUI();
	int RemoveUI();
	int ActivateUI();
	int DisableUI();
	int Window();
	int MoveToWindow();
	int Picking();

	//Graphics wrappers
	int RegisterSceneList();
	int UnRegister();
	int AddTexture();
	int AddStaticMesh();
	int AddText();
	int RemoveItemFromScene();
	int SetTexture();
	int SetMatrix();
	int SetCameraPos();
	int SetCameraTarget();
	int SetCameraUp();

	//Physics wrappers
	int SetAsKinematic();
	int ApplyCentralForce();
	int GetVelocityX();
	int GetVelocityY();
	int GetVelocityZ();
	int GetPostitionX();
	int GetPostitionY();
	int GetPostitionZ();
	int CreateCollisionSphere();
	int CreateCollisionBox();
	int CreateCollisionCylinder();
	int SetLinearVelocity();
	int DeletePhysicsObject();
	int SetShapePosition();
	int SetShapeRotation();
	int RotateShapeOnX();
	int RotateShapeOnY();
	int RotateShapeOnZ();

	//Sound wrappers
	int LoadSound();
	int Play();
	int Stop();
	int SetVolume();
	int CleanSound();

	//Entity wrappers
	int AddEntity();
	int RemoveEntity();

	int QuitGame();




	// Wrapped Functions
	void Print(int val1, int val2, int val3);
	int Add2Ints(int val1, int val2);
}
//=======================================================