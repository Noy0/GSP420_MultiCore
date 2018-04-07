#include "GraphicsCore.h"
#include "GraphicsMessage.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "StaticMeshManager.h"
#include "UIMessage.h"
#include "CoreManager.h"

GraphicsCore::GraphicsCore(IDirect3DDevice9** device, HWND hWnd)
{
	m_GEngine = new GraphicsEngine(hWnd);
	*device = m_GEngine->GetDevice();
	DD_ScreenTex* screentex;
	DD_ScreenText* screentext;
	DD_StaticMesh* staticmesh;
	m_pSceneManager = new SceneManager(&screentex, &screentext, &staticmesh);
	m_NextSceneListID = 1;
	RegisterSceneList(0, SCENE_SCREENTEX, (void*)screentex);
	RegisterSceneList(0, SCENE_SCREENTEXT, (void*)screentext);
	RegisterSceneList(0, SCENE_STATICMESH, (void*)staticmesh);
	m_GEngine->ChangePos(D3DXVECTOR3(50.0f, 0.0f, 0.0f));
}

GraphicsCore::~GraphicsCore()
{
	delete m_pSceneManager;
	delete m_GEngine;
}

void GraphicsCore::OnLaunch()
{
	gCoreMgr->HandleMessage(new SMessageUIRegisterCamera(&m_CameraOut));
}

void GraphicsCore::Update(float dt)
{
	CopyMessage();
	m_pSceneManager->Begin();
	GraphicsManagerMsg();
	m_pSceneManager->Export();
	
	GraphicsEngineMsg();
	DrawAllItems();

	CoreMsg();
	Export();
}

void GraphicsCore::RegisterSceneList(int* idOut, int type, void* item)
{
	if(item == 0)
		return;
	switch(type)
	{
	case SCENE_SCREENTEX:
		{
			if(idOut)
				*idOut = m_NextSceneListID;
			pair<int, DD_ScreenTex*> temp;
			temp.first = m_NextSceneListID++;
			temp.second = (DD_ScreenTex*)item;
			m_ScreenTexList.push_back(temp);
			break;
		}
	case SCENE_SCREENTEXT:
		{
			if(idOut)
				*idOut = m_NextSceneListID;
			pair<int, DD_ScreenText*> temp;
			temp.first = m_NextSceneListID++;
			temp.second = (DD_ScreenText*)item;
			m_ScreenTextList.push_back(temp);
			break;
		}
	case SCENE_STATICMESH:
		{
			if(idOut)
				*idOut = m_NextSceneListID;
			pair<int, DD_StaticMesh*> temp;
			temp.first = m_NextSceneListID++;
			temp.second = (DD_StaticMesh*)item;
			m_StaticMeshList.push_back(temp);
			break;
		}
	default:
		{
		}
	}
}

void GraphicsCore::GraphicsManagerMsg()
{
	queue<SMessage*> tempQueue;
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_REGISTERSCENELIST:
			{
				SMessageRegisterSceneList* msgex = (SMessageRegisterSceneList*)msg;
				RegisterSceneList(msgex->IDOut, msgex->Type, msgex->Item);
				delete msg;
				break;
			}
		case MSG_ADDTEXTURE:
			{
				SMessageAddTexture* msgex = (SMessageAddTexture*)msg;
				int ID = m_pSceneManager->AddItem(SCENE_SCREENTEX, msgex->ID);
				m_pSceneManager->SwapTexture(msgex->ID, msgex->Texture);
				D3DXMATRIX m;
				D3DXMatrixTranslation(&m, msgex->Position.x, msgex->Position.y, msgex->Position.z);
				m_pSceneManager->SetItemMatrix(msgex->ID, m);
				delete msg;
				break;
			}
		case MSG_ADDSTATICMESH:
			{
				SMessageAddStaticMesh* msgex = (SMessageAddStaticMesh*)msg;
				int ID = m_pSceneManager->AddItem(::SCENE_STATICMESH, msgex->ID);
				m_pSceneManager->SetItemMatrix(&ID, msgex->Transform);
				m_pSceneManager->SwapMesh(&ID, msgex->Mesh);
				delete msg;
				break;
			}
		case MSG_CHANGEMATRIX:
			{
				SMessageSetMatrix* msgex = (SMessageSetMatrix*) msg;
				m_pSceneManager->SetItemMatrix((msgex->ID), msgex->Matrix);
				delete msg;
				break;
			}
		default:
			{
				tempQueue.push(m_QueueBuffer.front());
				break;
			}
		}

		m_QueueBuffer.pop();
	}
	m_QueueBuffer = tempQueue;
}

void GraphicsCore::GraphicsEngineMsg()
{
	queue<SMessage*> tempQueue;
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_CHANGEUP:
			{
				SMessageCameraUp* msgx = (SMessageCameraUp*) msg;
				m_GEngine->ChangeUP(msgx->CameraUp);
				delete msg;
				break;
			}
		case MSG_CHANGELOOK:
			{
				SMessageCameraLook* msgx = (SMessageCameraLook*)msg;
				m_GEngine->ChangeLook(msgx->CameraLook);
				delete msg;
				break;
			}
		case MSG_CHANGEPOS:
			{
				SMessageCameraPos* msgx = (SMessageCameraPos*)msg;
				m_GEngine->ChangePos(msgx->CameraPos);
				delete msg;
				break;
			}
		case MSG_ROTATECAMERA:
			{
				SMessageRotateCamera* msgx = (SMessageRotateCamera*)msg;
				m_GEngine->RotationCamera(msgx->Angle);
				delete msg;
				break;
			}
		default:
			{
				tempQueue.push(m_QueueBuffer.front());
				break;
			}
		}
		m_QueueBuffer.pop();
	}
	m_QueueBuffer = tempQueue;
}

void GraphicsCore::DrawAllItems()
{
	m_GEngine->PreRender();
	vector<pair<int, D3DXMATRIX>> ScreenTexData;
	vector<DD_ScreenTextData> ScreenTextData;
	vector<DD_StaticMeshData> StaticMeshData;

	for(int i = 0; i < m_ScreenTexList.size(); ++i)
	{
		m_ScreenTexList[i].second->Copy(ScreenTexData);
		for(int j = 0; j < ScreenTexData.size(); ++j)
		{
			m_GEngine->RenderTexture(gTextureMgr->GetItem(ScreenTexData[j].first), ScreenTexData[j].second);
		}
	}

	for(int i = 0; i < m_ScreenTextList.size(); ++i)
	{
		m_ScreenTextList[i].second->Copy(ScreenTextData);
		for(int j = 0; j < ScreenTextData.size(); ++j)
		{
			//Draw Text here
			m_GEngine->RenderFont(gFontMgr->GetItem(ScreenTextData[j].FontID), ScreenTextData[j].Text, 
				ScreenTextData[j].Color, ScreenTextData[j].Transform);
		}
	}

	for(int i = 0; i < m_StaticMeshList.size(); ++i)
	{
		m_StaticMeshList[i].second->Copy(StaticMeshData);
		for(int j = 0; j < StaticMeshData.size(); ++j)
		{
			SMeshData* mesh = gStaticMeshMgr->GetItem(StaticMeshData[j].MeshID);
			//Draw static mesh
			if(StaticMeshData[j].TextureID)
				m_GEngine->RenderMeshTex(mesh->MeshSys, gTextureMgr->GetItem(StaticMeshData[j].TextureID), 
				mesh->Material, StaticMeshData[j].Transform);
			else
			{
				m_GEngine->RenderMesh(mesh->MeshSys,mesh->Texture, mesh->Material, StaticMeshData[j].Transform);
			}
		}
	}
	m_GEngine->PostRender();
}

void GraphicsCore::CoreMsg()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		}
		delete m_QueueBuffer.front();
		m_QueueBuffer.pop();
	}
}

void GraphicsCore::Export()
{
	DDCameraData camera;
	camera.Position = m_GEngine->GetCameraPos();
	camera.Target = m_GEngine->GetCameraTarget();
	camera.Up = m_GEngine->GetCameraUp();
	camera.Width = m_GEngine->GetScreenWidth();
	camera.Height = m_GEngine->GetScreenHeight();
	camera.Projection = m_GEngine->GetProjection();
	camera.View = m_GEngine->GetViewMatrix();
	m_CameraOut.Overwrite(camera);
}