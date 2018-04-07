#include "FunctionManager.h"
#include "..\\..\\..\\CoreManager.h"
#include "..\\..\\..\\SoundMessage.h"
#include "..\\..\\..\\EntityMessage.h"
#include "..\\..\\..\\UIMessage.h"
#include "..\\..\\..\\GameMessage.h"
#include "..\\..\\..\\AI_Message.h"
#include "..\\..\\..\\GraphicsMessage.h"
#include "..\\..\\..\\SystemMessage.h"
#include <d3dx9.h>


CFunctionManager* CFunctionManager::_instance = 0;
CFunctionManager::CFunctionManager(){

	mWrappers[ "HelloWorld" ] = &Wrappers::HelloWorld;
	mWrappers[ "TypeName" ]   = &Wrappers::TypeYourName;
	mWrappers[ "PrintValue"] = &Wrappers::PrintValue;
	mWrappers[ "Add2Ints" ] = &Wrappers::_Add2Ints;
	
	//Networking wrappers
	mWrappers[ "SwitchToServer"] = &Wrappers::SwitchToServer;
	mWrappers[ "SwitchToClient"] = &Wrappers::SwitchToClient;
	mWrappers[ "SendData"] = &Wrappers::SendData;
	mWrappers[ "GetLastMessage"] = &Wrappers::GetLastMessage;
	//mWrappers[ "GetMessage"] = &Wrappers::GetMessage;
	//UI wrappers
	mWrappers[ "AddUI"] = &Wrappers::AddUI;
	mWrappers[ "RemoveUI"] = &Wrappers::RemoveUI;
	mWrappers[ "ActivateUI"] = &Wrappers::ActivateUI;
	mWrappers[ "DisableUI"] = &Wrappers::DisableUI;
	mWrappers[ "Window"] = &Wrappers::Window;
	mWrappers[ "MoveToWindow"] = &Wrappers::MoveToWindow;
	mWrappers[ "Picking"] = &Wrappers::Picking;
	//Graphics wrappers
	mWrappers[ "RegisterSceneList"] = &Wrappers::RegisterSceneList;
	mWrappers[ "UnRegister"] = &Wrappers::UnRegister;
	mWrappers[ "AddTexture"] = &Wrappers::AddTexture;
	mWrappers[ "AddStaticMesh"] = &Wrappers::AddStaticMesh;
	mWrappers[ "AddText"] = &Wrappers::AddText;
	mWrappers[ "RemoveItemFromScene"] = &Wrappers::RemoveItemFromScene;
	mWrappers[ "SetTexture"] = &Wrappers::SetTexture;
	mWrappers[ "SetMatrix"] = &Wrappers::SetMatrix;
	mWrappers[ "SetCameraPos"] = &Wrappers::SetCameraPos;
	mWrappers[ "SetCameraTarget"] = &Wrappers::SetCameraTarget;
	mWrappers[ "SetCameraUp"] = &Wrappers::SetCameraUp;
	//Physics wrappers
	mWrappers[ "SetAsKinematic"] = &Wrappers::SetAsKinematic;
	mWrappers[ "ApplyForce"] = &Wrappers::ApplyCentralForce;
	mWrappers[ "GetVelocityX"] = &Wrappers::GetVelocityX;
	mWrappers[ "GetVelocityY"] = &Wrappers::GetVelocityY;
	mWrappers[ "GetVelocityZ"] = &Wrappers::GetVelocityZ;
	mWrappers[ "GetPostitionX"] = &Wrappers::GetPostitionX;
	mWrappers[ "GetPostitionY"] = &Wrappers::GetPostitionY;
	mWrappers[ "GetPostitionZ"] = &Wrappers::GetPostitionZ;
	mWrappers[ "CreateCollisionSphere"] = &Wrappers::CreateCollisionSphere;
	mWrappers[ "CreateCollisionBox"] = &Wrappers::CreateCollisionBox;
	mWrappers[ "CreateCollisionCylinder"] = &Wrappers::CreateCollisionCylinder;
	mWrappers[ "SetVelocity"] = &Wrappers::SetLinearVelocity;
	mWrappers[ "DeletePhysicsObject"] = &Wrappers::DeletePhysicsObject;
	mWrappers[ "SetShapePosition"] = &Wrappers::SetShapePosition;
	mWrappers[ "SetShapeRotation"] = &Wrappers::SetShapeRotation;
	mWrappers[ "RotateShapeOnX"] = &Wrappers::RotateShapeOnX;
	mWrappers[ "SetCameraUp"] = &Wrappers::SetCameraUp;
	mWrappers[ "RotateShapeOnZ"] = &Wrappers::RotateShapeOnZ;
	//Sound wrappers
	mWrappers[ "LoadSound"] = &Wrappers::LoadSound;
	mWrappers[ "PlaySound"] = &Wrappers::Play;
	mWrappers[ "StopSound"] = &Wrappers::Stop;
	mWrappers[ "SetVolume"] = &Wrappers::SetVolume;
	mWrappers[ "CleanSound"] = &Wrappers::CleanSound;
	//Entity wrappers
	mWrappers[ "AddEntity"] = &Wrappers::AddEntity;
	mWrappers[ "RemoveEntity" ] = &Wrappers::RemoveEntity;
	mWrappers[ "QuitGame" ] = &Wrappers::QuitGame;
	//Make sure that this is last
	mWrappers[ "" ] = NULL;

	mCurrentID = 0;
}

CFunctionManager::~CFunctionManager(){

	mWrappers.clear();
}

CFunctionManager* CFunctionManager::Instance(){

	if( _instance == 0 )
		_instance = new CFunctionManager;
		
	return _instance;
}

FUNC CFunctionManager::GetFunction( string funcName ){

	if( mWrappers.find( funcName ) != mWrappers.end() )
		return mWrappers[funcName];

	return NULL;
}

bool CFunctionManager::FindFunction( string funcName)
{
	if( mWrappers.find( funcName ) != mWrappers.end() )
		return true;

	return false;
}

void CFunctionManager::Destroy(){

	delete _instance;
	_instance = 0;
}

void CFunctionManager::SetCurrentID( int newID ){

	mCurrentID = newID;
}
//==================================
// Define all wrapper functions here
//==================================

int Wrappers::HelloWorld(){

		cout << "Hello World" << endl;
		return 0;
}

int Wrappers::TypeYourName(){

	string temp;
	cout << "What is your name: ";
	std::cin >> temp;

	cout << temp << " is gay." << endl;
	return 0;
}

int Wrappers::PrintValue()

{
	Print(VarMgr->StackToInt(0), VarMgr->StackToInt(1), VarMgr->StackToInt(2));

	return 0;
}

void Wrappers::Print(int val1, int val2, int val3)
{
	cout << "The Values are: "<< val1<< " " << val2<< " " <<val3 <<endl;
}

//==================================

int Wrappers::_Add2Ints()
{
	int temp = Add2Ints(VarMgr->StackToInt(0),VarMgr->StackToInt(1));
	VarMgr->ClearStack();

	VarMgr->PushToStack(temp);
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::Add2Ints(int val1, int val2)
{
	return val1 + val2;
}


//Networking wrappers
int Wrappers::SwitchToServer()
{
	return 0;
}

int Wrappers::SwitchToClient()
{
	return 0;
}

int Wrappers::SendData()
{
	return 0;
}

int Wrappers::GetLastMessage()
{
	return 0;
}

//int Wrappers::GetMessage()
//{
//	return 0;
//}

//UI wrappers
int Wrappers::AddUI()
{
	SUIInfo tempUI;
	tempUI.ID = VarMgr->StackToInt(0);
	tempUI.type = (EUI)VarMgr->StackToInt(1);
	tempUI.Size.x = VarMgr->StackToInt(2);
	tempUI.Size.y = VarMgr->StackToInt(3);
	tempUI.Position.x = (float)VarMgr->StackToInt(4);
	tempUI.Position.y = (float)VarMgr->StackToInt(5);
	tempUI.Position.z = (float)VarMgr->StackToInt(6);
	tempUI.TextureNormal = VarMgr->StackToInt(7);
	tempUI.TextureMouseOver = VarMgr->StackToInt(8);
	tempUI.TexturePressed = VarMgr->StackToInt(9);

	gCoreMgr->HandleMessage(new SMessageAddUI(tempUI));
	//gCoreMgr->HandleMessage(new SMessageActivateUI(tempUI.ID));
	VarMgr->ClearStack();
	
	return 0;
}

int Wrappers::RemoveUI()
{
	return 0;
}

int Wrappers::ActivateUI()
{
	gCoreMgr->HandleMessage( new SMessageActivateUI( VarMgr->StackToInt(0) ) );
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::DisableUI()
{
	gCoreMgr->HandleMessage( new SMessageDisableUI( VarMgr->StackToInt(0) ) );
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::Window()
{
	return 0;
}

int Wrappers::MoveToWindow()
{
	return 0;
}

int Wrappers::Picking()
{
	return 0;
}

//Graphics wrappers
int Wrappers::RegisterSceneList()
{
	return 0;
}

int Wrappers::UnRegister()
{
	return 0;
}

int Wrappers::AddTexture()
{
	return 0;
}

int Wrappers::AddStaticMesh()
{
	return 0;
}

int Wrappers::AddText()
{
	return 0;
}

int Wrappers::RemoveItemFromScene()
{
	return 0;
}

int Wrappers::SetTexture()
{
	return 0;
}

int Wrappers::SetMatrix()
{
	return 0;
}

int Wrappers::SetCameraPos()
{
	return 0;
}

int Wrappers::SetCameraTarget()
{
	return 0;
}

int Wrappers::SetCameraUp()
{
	return 0;
}

//Physics wrappers

int Wrappers::SetAsKinematic()
{
	return 0;
}

int Wrappers::ApplyCentralForce()
{
	gCoreMgr->HandleMessage( new SMessageSetForce( FuncMgr->CurrentID(), D3DXVECTOR3(VarMgr->StackToFloat(0), VarMgr->StackToFloat(1), VarMgr->StackToFloat(2))));
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::GetVelocityX()
{
	return 0;
}

int Wrappers::GetVelocityY()
{
	return 0;
}
	
int Wrappers::GetVelocityZ()
{
	return 0;
}
	
int Wrappers::GetPostitionX()
{
	return 0;
}
	
int Wrappers::GetPostitionY()
{
	return 0;
}

int Wrappers::GetPostitionZ()
{
	return 0;
}

int Wrappers::CreateCollisionSphere()
{
	return 0;
}
	
int Wrappers::CreateCollisionBox()
{
	return 0;
}
	
int Wrappers::CreateCollisionCylinder()
{
	return 0;
}

int Wrappers::SetLinearVelocity()
{
	gCoreMgr->HandleMessage( new SMessageSetVelocity((FuncMgr->CurrentID()), D3DXVECTOR3(VarMgr->StackToFloat(0), VarMgr->StackToFloat(1), VarMgr->StackToFloat(2))));
	VarMgr->ClearStack();
	return 0;
}
	
int Wrappers::DeletePhysicsObject()
{
	return 0;
}

int Wrappers::SetShapePosition()
{
	return 0;
}

int Wrappers::SetShapeRotation()
{
	return 0;
}

int Wrappers::RotateShapeOnX()
{
	return 0;
}

int Wrappers::RotateShapeOnY()
{
	return 0;
}

int Wrappers::RotateShapeOnZ()
{
	
	return 0;
}

//Sound wrappers

int Wrappers::LoadSound()
{
	return 0;
}

int Wrappers::Play()
{
	gCoreMgr->HandleMessage( new SMessagePlay( VarMgr->StackToInt(0), VarMgr->StackToInt(1) ) );
	VarMgr->ClearStack();
	return 0;
}
	
int Wrappers::Stop()
{
	gCoreMgr->HandleMessage( new SMessageStop( VarMgr->StackToFloat(0) ) );
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::SetVolume()
{
	gCoreMgr->HandleMessage( new SMessageSetVol( VarMgr->StackToInt(0), VarMgr->StackToInt(1) ) );
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::CleanSound()
{
	return 0;
}

int Wrappers::AddEntity()
{
	D3DXVECTOR3 pos;
	int id = 0;

	pos.x = VarMgr->StackToFloat(1);
	pos.y = VarMgr->StackToFloat(2);
	pos.z = VarMgr->StackToFloat(3);

	gCoreMgr->HandleMessage(new SMessageAddEntity(&id,VarMgr->StackToInt(0),pos));
	VarMgr->ClearStack();
	return 0;
}

int Wrappers::RemoveEntity(){

	gCoreMgr->HandleMessage( new SMessageRemoveEntity( FuncMgr->CurrentID()) );
	return 0;
}

int Wrappers::QuitGame()
{
	gCoreMgr->HandleMessage(new SMessageQuitGame());
	return 0;
}