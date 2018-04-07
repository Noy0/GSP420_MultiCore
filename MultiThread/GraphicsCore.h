#ifndef _GRAPHICSCORE_H
#define _GRAPHICSCORE_H

#include "BaseCore.h"
//#include "GraphicsEngine.h"
#include "Display.h"
#include "SceneManager.h"
#include "DDCamera.h"

class GraphicsCore : public BaseCore
{
public:
	GraphicsCore(IDirect3DDevice9** device, HWND hWnd);
	~GraphicsCore();
private:
	void OnLaunch();
	void Update(float dt);

	void RegisterSceneList(int* idOut, int type, void* item);
	void UnregisterSceneList(int id);
	void GraphicsManagerMsg();
	void GraphicsEngineMsg();
	void DrawAllItems();
	void CoreMsg();
	void Export();

	//Shared data read
	vector<pair<int, DD_ScreenTex*>> m_ScreenTexList;
	vector<pair<int, DD_ScreenText*>> m_ScreenTextList;
	vector<pair<int, DD_StaticMesh*>> m_StaticMeshList;

	//Shared data write
	DD_Camera m_CameraOut;

	//Components
	SceneManager* m_pSceneManager;
	GraphicsEngine *m_GEngine;
	int m_NextSceneListID;
};

#endif