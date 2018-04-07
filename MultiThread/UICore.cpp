#include "UICore.h"
#include "UIMessage.h"
#include "CoreManager.h"
#include "GraphicsMessage.h"
#include "EScene.h"
#include <windows.h>
#include "GameMessage.h"

UICore::UICore(HWND hwnd) : m_hWnd(hwnd)
{
	m_Camera = 0;
	m_DDPicking = 0;
	m_EnablePicking = 0;
}

UICore::~UICore()
{
}

void UICore::OnLaunch()
{
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_SCREENTEX, (void*)&m_SceneTextureOut));
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_SCREENTEXT, (void*)&m_SceneTextOut));
}

void UICore::Update(float dt)
{
	CopyMessage();
	m_EnablePicking = true;
	m_UIManager.Update(dt);
	GetCursorPos(&m_MousePos);
	ScreenToClient(m_hWnd, &m_MousePos);
	if(m_UIManager.MouseOverCheck(m_MousePos) != 0)
		m_EnablePicking = false;
	UIMessage();

	vector<pair<int, D3DXMATRIX>> UISceneTexture;
	vector<DD_ScreenTextData> UISceneText;
	vector<pair<int, std::string>> UIText;

	m_UIManager.Export(&UISceneTexture, (vector<DD_ScreenTextData>*)&UISceneText, &UIText);
	m_SceneTextureOut.Overwrite(UISceneTexture);
	m_SceneTextOut.Overwrite(UISceneText);
}

void UICore::UIMessage()
{
	queue<SMessage*> temp;

	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_ADDUI:
			{
				SMessageAddUI* msgex = (SMessageAddUI*)(msg);
				m_UIManager.AddUI(msgex->Info);
				delete msg;
				break;
			}
		case MSG_REMOVEUI:
			{
				SMessageRemoveUI* msgex = (SMessageRemoveUI*)(msg);
				m_UIManager.RemoveUI(msgex->ID);
				delete msg;
				break;
			}
		case MSG_ACTIVATEUI:
			{
				SMessageActivateUI* msgex = (SMessageActivateUI*)(msg);
				m_UIManager.ActivateUI(msgex->ID);
				delete msg;
				break;
			}
		case MSG_DISABLEUI:
			{
				SMessageDisableUI* msgex = (SMessageDisableUI*)(msg);
				m_UIManager.DisableUI(msgex->ID);
				delete msg;
				break;
			}
		case MSG_WINDOW:
			{
				SMessageWindow* msgex = (SMessageWindow*)(msg);
				switch(msgex->Msg.message)
				{
				case WM_LBUTTONDOWN:
					{
						int result = m_UIManager.MouseButtonDown(0);
						if(result != 0)
						{
							//send message to game core
						}
						else
						{
							int picked = -1;
							if(m_Camera && m_DDPicking)
							{
								DDCameraData camera;
								m_Camera->Copy(camera);
								DDPickingData picking;
								m_DDPicking->Copy(picking);
								picked = m_Picking.picking(m_MousePos, camera.Projection, camera.View, picking.ID, picking.Position, picking.Halfscale, camera.Width, camera.Height);
								if(picked > -1)
								{
									gCoreMgr->HandleMessage(new SMessagePicked(picked));
								}
							}
						}
						break;
					}
				case WM_LBUTTONUP:
					{
						int result = m_UIManager.MouseButtonUp(0);
						if(result != 0)
						{
							//send message to game core
							gCoreMgr->HandleMessage(new SMessageClickedUI(result));
						}
						break;
					}
				case WM_CHAR:
					{
						int result = m_UIManager.KeyPress((char)(msgex->Msg.wParam));
						break;
					}
				case WM_KEYDOWN:
					{
						if(!m_UIManager.Typeing())
							gCoreMgr->HandleMessage(new SMessageGameKeyDown((int)msgex->Msg.wParam));
						break;
					}
				}
				delete msg;
				break;
			}
		case MSG_UIREGISTERCAMERA:
			{
				SMessageUIRegisterCamera* msgx = (SMessageUIRegisterCamera*) msg;
				m_Camera = msgx->Camera;
				delete msg;
				break;
			}
		case MSG_REGISTERPICKING:
			{
				SMessageRegisterPicking* msgx = (SMessageRegisterPicking*)msg;
				m_DDPicking = msgx->Picking;
				delete msg;
				break;
			}
		default:
			{
				temp.push(m_QueueBuffer.front());
				break;
			}
		}
		m_QueueBuffer.pop();
	}
}