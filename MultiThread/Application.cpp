#include "Application.h"
#include "CoreManager.h"
#include "GraphicsCore.h"
#include "UICore.h"
#include "GameCore.h"
#include "EntityCore.h"
#include "SoundCore.h"
#include "ScriptingCore\ScriptingCore.h"
#include "AI_CORE.h"
#include "Networking\NetworkingCore.h"
#include "CoreList.h"
#include "TextureManager.h"
#include "StaticMeshManager.h"
#include "FontManager.h"
#include "EntityPropertyManager.h"
#include "ResourceLoader.h"
#include "SystemMessage.h"
#include "SteeringManager.h"

//Testing only
//#include "GraphicsMessage.h"
//#include "DCContainer.h"
#include "UIMessage.h"
#include "EntityMessage.h"
#include "EntityPropertyList.h"


SampleApp* gApp = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(!gApp)
		return DefWindowProc(hwnd, msg, wparam, lparam);
	else
		return gApp->ProcessWindowMsg(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

SampleApp::SampleApp(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	//Initialize app stuff here

	//Initialize the window
	InitWindow(800, 600, WS_OVERLAPPEDWINDOW);

	//Create cores
		//CoreManager
	CoreManager::Create();
		//Graphics
	IDirect3DDevice9* Gdevice;
	GraphicsCore* GCore = new GraphicsCore(&Gdevice, m_hWnd);
	gCoreMgr->AddCore(GCore, CORE_GRAPHICS);
	gCoreMgr->SetCoreFramesPerSecond(CORE_GRAPHICS, FPS_CORE_GRAPHICS);
		//UI
	UICore* UI = new UICore(m_hWnd);
	UI->SetFramesPerSecond(FPS_CORE_UI);
	gCoreMgr->AddCore(UI, CORE_UI);
		//Game
	GameCore* game = new GameCore();
	game->SetFramesPerSecond(FPS_CORE_GAME);
	gCoreMgr->AddCore(game, CORE_GAME);
		//Entity
	EntityCore* entity = new EntityCore();
	entity->SetFramesPerSecond(FPS_CORE_ENTITY);
	gCoreMgr->AddCore(entity, CORE_ENTITY);
		//Sound
	SoundCore* sound = new SoundCore(m_hWnd);
	sound->SetFramesPerSecond(FPS_CORE_SOUND);
	gCoreMgr->AddCore(sound, CORE_SOUND);
		//Scripting
	CScriptingCore* scripting = new CScriptingCore();
	scripting->SetFramesPerSecond(FPS_CORE_SCRIPTING);
	gCoreMgr->AddCore(scripting, CORE_SCRIPTING);
		//AI
	AI_Core* AI = new AI_Core();
	AI->SetFramesPerSecond(FPS_CORE_AI);
	gCoreMgr->AddCore(AI, CORE_AI);
		//Networking
	NetCore* network = new NetCore();
	network->SetFramesPerSecond(FPS_CORE_NETWORKING);
	gCoreMgr->AddCore(network, CORE_NETWORKING);
	//Load Resources
		//Initialize each resource manager
	TextureManager::Initialize(Gdevice);
	StaticMeshManager::Initailize(Gdevice);
	FontManager::Initialize(Gdevice);
	EntityPropertyMgr::Initialize();
	SteeringConstantsMgr::Initialize();
		//Load with loader
	ResourceLoader::Load();

	//Launch all cores
	gCoreMgr->Launch();
}

void SampleApp::InitWindow(int width, int length, DWORD style)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = 0;
	wc.hInstance = m_hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "SampleWindow";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	RECT r = {100, 100, width, length};

	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);

	m_hWnd = CreateWindow("SampleWindow", "Sample App", style, r.left, r.top, r.right, r.bottom, 0, 0, m_hInstance, 0);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

void SampleApp::Run()
{
	MSG msg;
	//Calculate sec per count
	__int64 countspersec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countspersec);
	float secpercount = 1.0f / (float)countspersec;

	//Get timestamp
	__int64 prevtimestamp;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevtimestamp);
	__int64 currenttimestamp;
	float dt;

	msg.message = WM_NULL;
	//Main loop
	while(msg.message != WM_QUIT)
	{
		//Handle window messages
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			gCoreMgr->HandleMessage(new SMessageWindow(msg));
			DispatchMessage(&msg);
		}
		else
		{
			//Update program
			QueryPerformanceCounter((LARGE_INTEGER*)&currenttimestamp);
			dt = (float)(currenttimestamp - prevtimestamp) * secpercount;
			//Update here
			//gCoreMgr->Update(dt);
			prevtimestamp = currenttimestamp;
		}
	}
}

LRESULT SampleApp::ProcessWindowMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_ACTIVATE: //When user activates or deactivate this window
		{
		}
		break;
	case WM_CLOSE: //Close window
		{
			DestroyWindow(m_hWnd);
			gCoreMgr->HandleMessage(new SMessageEndApp());
			return 0;
		}
		break;
	case WM_DESTROY: //End program
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

SampleApp::~SampleApp()
{
}