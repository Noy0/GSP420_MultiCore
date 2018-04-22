#include "GameCore.h"
#include "GameMessage.h"
#include "CoreManager.h"
#include "GraphicsMessage.h"
#include "AI_Message.h"
#include "EntityMessage.h"
#include "UIList.h"
#include "FontList.h"
#include "ScriptingCore\ScriptMessages.h"
#include <d3dx9.h>
#include "GameState.h"
#include "Networking\NetCoreMessage.h"

GameCore::GameCore()
{
	m_CameraPosition = D3DXVECTOR3(0,-1, -50);
	m_ViewAngle = 0;
	for(int i = 0; i < 4; ++i)
		m_PlayerScore[i] = 0;
	m_Turn = 1;
	m_PositionIn = 0;
	m_Hosting = false;
	m_GameState = 0;
	m_SubState = SGS_SHOOTING;
	
}

GameCore::~GameCore()
{
}

void GameCore::OnLaunch()
{
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_SCREENTEXT, &m_TextOut));
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_SCREENTEX, &m_TexOut));
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_STATICMESH, &m_StaticMeshOut));
	gCoreMgr->HandleMessage(new SMessageCameraPos(m_CameraPosition));
	gCoreMgr->HandleMessage(new SMessageCameraLook(D3DXVECTOR3(0,0,0)));
	gCoreMgr->HandleMessage(new SMessageCameraUp(D3DXVECTOR3(0,1,0)));
	gCoreMgr->HandleMessage( new SMessageRunScript("Create_UI.txt") );
}

void GameCore::Update(float dt)
{
	CopyMessage();

	switch(m_GameState)
	{
	case GS_MENU:
		{
			MenuUpdate(dt);
			break;
		}
	case GS_SINGLE:
		{
			SingleUpdate(dt);
			break;
		}
	case GS_HOSTING:
		{
			HostingUpdate(dt);
			break;
		}
	case GS_CLIENT:
		{
			ClientUpdate(dt);
			break;
		}
		//sprintf(num, "Physics Core SPF: %f", gCoreMgr->GetLastSPF(CORE_ENTITY));
		//D3DXMATRIX m;
		//D3DXMatrixTranslation(&m, 0, 0, 0);
		//temp.FontID = FONT_CHAT;
		//temp.Text = num;
		//temp.Transform = m;
		//temp.Color = D3DXCOLOR(0, 0, 0, 1);
		//textOut.push_back(temp);
		//sprintf(num, "Graphics Core SPF: %f", gCoreMgr->GetLastSPF(CORE_GRAPHICS));
		//D3DXMatrixTranslation(&m, 0, 20, 0);
		//temp.Text = num;
		//temp.Transform = m;
		//textOut.push_back(temp);
		//sprintf(num, "AI Core SPF: %f", gCoreMgr->GetLastSPF(CORE_AI));
		//D3DXMatrixTranslation(&m, 0, 40, 0);
		//temp.Text = num;
		//temp.Transform = m;
		//textOut.push_back(temp);
		//sprintf(num, "UI Core SPF: %f", gCoreMgr->GetLastSPF(CORE_UI));
		//D3DXMatrixTranslation(&m, 0, 60, 0);
		//temp.Text = num;
		//temp.Transform = m;
		//textOut.push_back(temp);
		//sprintf(num, "Scripting Core SPF: %f", gCoreMgr->GetLastSPF(CORE_SCRIPTING));
		//D3DXMatrixTranslation(&m, 0, 80, 0);
		//temp.Text = num;
		//temp.Transform = m;
		//textOut.push_back(temp);
		//sprintf(num, "Game Core SPF: %f", gCoreMgr->GetLastSPF(CORE_GAME));
		//D3DXMatrixTranslation(&m, 0, 100, 0);
		//temp.Text = num;
		//temp.Transform = m;
		//textOut.push_back(temp);
		//sprintf(num, "Network Core SPF: %f", gCoreMgr->GetLastSPF(CORE_NETWORKING));
		//D3DXMatrixTranslation(&m, 0, 100, 0);
		//temp.Text = num;
		//temp.Transform = m;
		//textOut.push_back(temp);

	}
	//Exports
	m_TextOut.Overwrite(m_TextOutBuffer);
	m_CameraPosition.x = cos(m_ViewAngle) * 40;
	m_CameraPosition.z = sin(m_ViewAngle) * 40;
	gCoreMgr->HandleMessage(new SMessageCameraPos(m_CameraPosition));
	//gCoreMgr->HandleMessage(new SMessageCameraPos(D3DXVECTOR3(0,10,20)));
	//gCoreMgr->HandleMessage(new SMessageCameraUp(D3DXVECTOR3(1, 0, 0)));
}

void GameCore::ExecuteMessage()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = BaseCore::m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_CLICKEDUI:
			{
				SMessageClickedUI* msgex = (SMessageClickedUI*)msg;
				ClickedUI(msgex->UI_ID);
				delete msg;
				break;
			}
		case MSG_PICKED:
			{
				SMessagePicked* msgx = (SMessagePicked*)msg;
				Picked(msgx->ID);
				delete msg;
				break;
			}
		case MSG_GAMEKEYDOWN:
			{
				SMessageGameKeyDown* msgx = (SMessageGameKeyDown*)msg;
				KeyDown(msgx->VK);
				delete msg;
				break;
			}
		case MSG_GAMEREGISTERPOSITION:
			{
				SMessageGameRegisterPosition* msgx = (SMessageGameRegisterPosition*)msg;
				m_PositionIn = msgx->pPositions;
				delete msg;
				break;
			}
		default:
			delete msg;
		}
		m_QueueBuffer.pop();
	}

}

void GameCore::ClickedUI(int id)
{
	static int id3;
	switch(id)
	{
	case UI_SINGLE:
		{
			gCoreMgr->HandleMessage( new SMessageRunScript( "UI_Button_click.txt" ) );
			m_GameState = GS_SINGLE;
			m_SubState = SGS_SHOOTING;
			break;
		}
	case UI_LEFTARROW:
		{
			m_ViewAngle -= 0.05;
			//gCoreMgr->HandleMessage( new SMessageRunScript( "leftarrow.txt" ) );
			break;
		}
	case UI_RIGHTARROW:
		{
			m_ViewAngle += 0.05;
			//gCoreMgr->HandleMessage( new SMessageRunScript( "rightarrow.txt" ) );
			break;
		}
	case UI_SHOOT:
		{
			static int temp;
			gCoreMgr->HandleMessage( new SMessageRunScript( "shoot.txt" ) );
			gCoreMgr->HandleMessage( new SMessageAddEntity( &temp, EP_PLAYERMARBLE, m_CameraPosition ) );
			gCoreMgr->HandleMessage( new SMessageSetVelocity( &temp, (D3DXVECTOR3(0,0,0) - m_CameraPosition) * 1 ) );
			break;
		}
		//For Hosting
	case UI_HOST:
		{
			gCoreMgr->HandleMessage( new SMessageRunScript( "UI_Button_click.txt" ) );
			m_Hosting = true;
			m_GameState = GS_HOSTING;
			m_SubState = SGS_WAITONCLIENT;
			gCoreMgr->HandleMessage( new SMessageSwitchToServer());
			break;
		}
	case UI_CLIENT:
		{
			m_GameState = GS_CLIENT;
			break;
		}
	}
}

void GameCore::Picked(int id)
{
	gCoreMgr->HandleMessage(new SMessageIncScriptFlag1(id));
}

void GameCore::KeyDown(int vk)
{
	switch(vk)
	{
	case VK_LEFT:
		{
			m_ViewAngle -= 0.1;
			break;
		}
	case VK_RIGHT:
		{
			m_ViewAngle += 0.1;
			break;
		}
	case VK_DOWN:
		{
			if(m_CameraPosition.y > -4.0)
				m_CameraPosition.y -= 0.1;
			break;
		}
	case VK_UP:
		{
			if(m_CameraPosition.y < 20.0)
				m_CameraPosition.y += 0.1;
			break;
		}
	case VK_SPACE:
		{
			static int temp = 100;
			gCoreMgr->HandleMessage( new SMessageRunScript( "shoot.txt" ) );
			gCoreMgr->HandleMessage( new SMessageAddEntity( &temp, EP_PLAYERMARBLE, m_CameraPosition ) );
			gCoreMgr->HandleMessage( new SMessageSetVelocity( &temp, (D3DXVECTOR3(0,0,0) - m_CameraPosition) * 1 ) );
			break;
		}
	}
}

void GameCore::MenuUpdate(float dt)
{
	ExecuteMessage();
}

void GameCore::SingleUpdate(float dt)
{
	static vector<DDGameData> tempPos;
	switch(m_SubState)
	{
	case SGS_SHOOTING:
		{
			ExecuteMessageShoot();
		
			break;
		}
	case SGS_HOLD:
		{
			static float timer = 0;
			timer += dt;
			if(m_Turn == 2)
			{
				if(timer >= 7)
				{
					--m_Turn;
					m_Turn %= 4;
					timer = 0;
					gCoreMgr->HandleMessage(new SMessageCameraLook((D3DXVECTOR3(0,0,0))));
					m_SubState = SGS_SHOOTING;
				}
			}
			else
			{
				if(timer >= 7)
				{
					timer = 0;
					m_SubState = SGS_POSTMOVE;
					
				}
			}
				
			ExecuteMessageNoAccess();
			break;
		}
	case SGS_AI:
		{
			ExecuteMessageAI();
			break;
		}
	case SGS_SPINNING:
		{
			static float timer = 0;
			static float randomtime = 0;
			if(timer == 0.0f)
			{
				randomtime = 5 + rand() % 5;
				
				if(m_PositionIn)
				{
					m_PositionIn->Copy(tempPos);
					for (int i = 0; i < tempPos.size(); ++i)
					{
						if (tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
							tempPos[i].Type == EP_LARGEMARBLE )
									
					
						{
						
							if (tempPos[i].Type == EP_SMALLMARBLE) {
								gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 1));
							}
							else if (tempPos[i].Type == EP_MEDIUMMARBLE) {
								gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 1));
							}
							else 
								gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 1));
								

							
							//++m_PlayerScore[m_Turn];
						}
					}
				}
			}
			timer += dt;
			m_ViewAngle += dt;
			if(timer >= randomtime)
			{
				if(m_Turn == 0)
				{
					m_SubState = SGS_SHOOTING;
				}
				else
				{
					m_SubState = SGS_AI;
				}
				timer = 0.0f;
				if(m_PositionIn)
				{
					m_PositionIn->Copy(tempPos);
					for (int i = 0; i < tempPos.size(); ++i)
					{
						if (tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
							tempPos[i].Type == EP_LARGEMARBLE )
						{
							
							if (tempPos[i].Type == EP_SMALLMARBLE) {
							gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 5));
							}
							else if (tempPos[i].Type == EP_MEDIUMMARBLE) {
							gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 5));
							}
							else
							gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 5));
						
							
							++m_PlayerScore[m_Turn];
						}
					}
				}
				m_SubState = SGS_AI;
				//Send message to fuzzy logic to request AI action
				LookAt = D3DXVECTOR3(0.0,0.0,0.0);
				gCoreMgr->HandleMessage(new SMessageAITurn(m_CameraPosition, (LookAt - m_CameraPosition)));
	
				
				
				//m_SubState = SGS_SHOOTING;
				m_SubState = SGS_HOLD;
				
			}
			ExecuteMessageNoAccess();
			break;
		}
	case SGS_POSTMOVE:
		{
			ExecuteMessageNoAccess();
			if(m_PositionIn)
			{
				static vector<DDGameData> tempPos;
				m_PositionIn->Copy(tempPos);
				for(int i = 0; i < tempPos.size(); ++i)
				{
					if ((tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
						tempPos[i].Type == EP_LARGEMARBLE || tempPos[i].Type == EP_PLAYERMARBLE)
						&& tempPos[i].Position.y < -20.0f)
					{
						gCoreMgr->HandleMessage(new SMessageRemoveEntity(&(tempPos[i].ID)));

						m_PlayerScore[m_Turn] += 10;
					}
				}

				m_BallCount = 0;
				//int ballCount{ 0 };
				for (int i = 0; i < tempPos.size(); ++i)
				{
					
					if ((tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
						tempPos[i].Type == EP_LARGEMARBLE))
					{
						m_BallCount += 1;
					}
				}
				if (m_BallCount == 0)
				{
					m_GameOver = true;
					m_SubState = SGS_GAMEOVER;
					break;
				}
			}
			//Change player
			
			++m_Turn;
			m_Turn %= 4;
			m_SubState = SGS_SPINNING;
			break;
		}
	case SGS_GAMEOVER:
		{
		m_Quit = true;
		}
	}
	m_TextOutBuffer.clear();
	char num[40];
	DD_ScreenTextData temp;
	//==============Player's score==================
	sprintf(num, "PLAYER SCORE: %i", m_PlayerScore[m_Turn]); 
	if ((m_PlayerScore[1] > m_PlayerScore[2]) && (m_BallCount == 0) && m_GameOver == true) { 
		sprintf(num, "PLAYER SCORE: %i \nYou Win", m_PlayerScore[1]);
	} 
	if ((m_PlayerScore[2] > m_PlayerScore[1]) && (m_BallCount == 0) && m_GameOver == true) {
		sprintf(num, "PLAYER SCORE: %i \nYou Lose", m_PlayerScore[1]); 
	}

	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, 0, 0, 0);
	temp.FontID = FONT_CHAT;
	temp.Text = num;
	temp.Transform = m;
	temp.Color = D3DXCOLOR(0, 0, 0, 1);
	m_TextOutBuffer.push_back(temp);
	//==============================================
	//===============Current Player=================
	sprintf(num, "CURRENT PLAYER: %i", m_Turn);
	D3DXMatrixTranslation(&m, 540, 0, 0);
	temp.FontID = FONT_CHAT;
	temp.Text = num;
	temp.Transform = m;
	temp.Color = D3DXCOLOR(0, 0, 0, 1);
	m_TextOutBuffer.push_back(temp);
	//==============================================
	//Power Bar

}

void GameCore::HostingUpdate(float dt)
{
	static vector<DDGameData> tempPos;
	if(m_PositionIn)
	{
		m_PositionIn->Copy(tempPos);
	}
	switch(m_SubState)
	{
	case SGS_SHOOTING:
		{
			if(m_Turn == 0)
				ExecuteMessageShoot();
			else
			{
				ExecuteMessageNoAccess();
			}
			break;
		}
	case SGS_HOLD:
		{
			static float timer = 0;
			timer += dt;
			if(timer >= 7)
			{
				timer = 0;
				m_SubState = SGS_POSTMOVE;
			}
			ExecuteMessageNoAccess();
			break;
		}
	case SGS_SPINNING:
		{
			static float timer = 0;
			static float randomtime = 0;
			if(timer == 0.0f)
			{
				randomtime = 5 + rand() % 5;				
				for(int i = 0; i < tempPos.size(); ++i)
				{
					int j = tempPos[i].ID;
					if(tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
						tempPos[i].Type == EP_LARGEMARBLE)
					{
						gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 1));
						++m_PlayerScore[m_Turn];
					}
				}				
			}
			timer += dt;
			m_ViewAngle += dt;
			if(timer >= randomtime)
			{
				if(m_Turn == 0)
				{
					m_SubState = SGS_SHOOTING;
				}
				else
				{
					//m_SubState = SGS_AI;
				}
				timer = 0.0f;
				for(int i = 0; i < tempPos.size(); ++i)
				{
					if(tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
						tempPos[i].Type == EP_LARGEMARBLE)
					{
						gCoreMgr->HandleMessage(new SMessageSetSteering(&(tempPos[i].ID), 0));
						++m_PlayerScore[m_Turn];
					}
				}
				m_SubState = SGS_AI;
				//Send message to fuzzy logic to request AI action
			}
			ExecuteMessageNoAccess();
			break;
		}
	case SGS_POSTMOVE:
		{
			ExecuteMessageNoAccess();
			for(int i = 0; i < tempPos.size(); ++i)
			{
				if((tempPos[i].Type == EP_SMALLMARBLE || tempPos[i].Type == EP_MEDIUMMARBLE ||
					tempPos[i].Type == EP_LARGEMARBLE || tempPos[i].Type == EP_PLAYERMARBLE) 
					&& tempPos[i].Position.y < -20.0f)
				{
					gCoreMgr->HandleMessage(new SMessageRemoveEntity(&(tempPos[i].ID)));
					++m_PlayerScore[m_Turn];
				}
			}
			//Change player
			++m_Turn;
			m_Turn %= 4;
			m_SubState = SGS_SPINNING;
			break;
		}
	case SGS_WAITONCLIENT:
		{
			static int clientcount = 0;
			if(clientcount < 4)
			{
				//Request for client count
			}
			else
			{
				m_SubState = SGS_SHOOTING;
				m_Turn = 0;
			}
			break;
		}
	}
	m_TextOutBuffer.clear();
	char num[40];
	DD_ScreenTextData temp;
	//==============Player's score==================
	sprintf( num, "PLAYER SCORE: %i", m_PlayerScore[m_Turn]);
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, 0, 0, 0);
	temp.FontID = FONT_CHAT;
	temp.Text = num;
	temp.Transform = m;
	temp.Color = D3DXCOLOR(0, 0, 0, 1);
	m_TextOutBuffer.push_back(temp);
	//==============================================
	//===============Current Player=================
	sprintf(num, "CURRENT PLAYER: %i", m_Turn);
	D3DXMatrixTranslation(&m, 540, 0, 0);
	temp.FontID = FONT_CHAT;
	temp.Text = num;
	temp.Transform = m;
	temp.Color = D3DXCOLOR(0, 0, 0, 1);
	m_TextOutBuffer.push_back(temp);
	//==============================================
	//Power Bar


	SGameNetData netdata;
	netdata.ID = 0;
	netdata.Header.m_Type = NDT_UPDATESCENE;
	pair<int, D3DXVECTOR3> tempdata;
	for(int i = 0; i < tempPos.size(); ++i)
	{
		tempdata.first = tempPos[i].Type;
		tempdata.second = tempPos[i].Position;
	}
	netdata.Header.m_Length = sizeof(netdata);
	char *ch = new char[256];
	memcpy((void*)ch, (void*)&netdata, sizeof(netdata));
	gCoreMgr->HandleMessage(new SMessageSendData(ch, sizeof(netdata)));
}

void GameCore::ClientUpdate(float dt)
{
}

void GameCore::ExecuteMessageMenu()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = BaseCore::m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_CLICKEDUI:
			{
				SMessageClickedUI* msgex = (SMessageClickedUI*)msg;
				//ClickedUI(msgex->UI_ID);
				switch(msgex->UI_ID)
				{
				case UI_SINGLE:
					{
						gCoreMgr->HandleMessage( new SMessageRunScript( "UI_Button_click.txt" ) );
						m_GameState = GS_SINGLE;
						break;						 
					}
				case UI_HOST:
					{
						m_Hosting = true;
						m_GameState = GS_HOSTING;
						//script for hosting
						gCoreMgr->HandleMessage( new SMessageSwitchToServer());
						break;
					}
				case UI_CLIENT:
					{
						m_GameState = GS_CLIENT;
						break;
					}
				}
				delete msg;
				break;
			}
		case MSG_GAMEREGISTERPOSITION:
			{
				SMessageGameRegisterPosition* msgx = (SMessageGameRegisterPosition*)msg;
				m_PositionIn = msgx->pPositions;
				delete msg;
				break;
			}
		default:
			delete msg;
		}
		m_QueueBuffer.pop();
	}
}

void GameCore::ExecuteMessageNoAccess()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_RECEIVEDFROMNET:
			{
				SMessageReceivedFromNet* msgx = (SMessageReceivedFromNet*) msg;
				SGameNetData* data = (SGameNetData*)msgx->Data;
				switch(data->Header.m_Type)
				{
				case NDT_UPDATESCENE:
					{
						break;
					}
				}
				delete msg;
				break;
			}
		default:
			{
				delete msg;
				break;
			}
		}
		m_QueueBuffer.pop();
	}
}

void GameCore::ExecuteMessageShoot()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_CLICKEDUI:
			{
				SMessageClickedUI* msgex = (SMessageClickedUI*)msg;
				ClickedUI(msgex->UI_ID);
				delete msg;
				break;
			}
		case MSG_GAMEKEYDOWN:
			{
				SMessageGameKeyDown* msgx = (SMessageGameKeyDown*)msg;
				switch(msgx->VK)
				{
				case VK_LEFT:
					{
						m_ViewAngle -= 0.1;
						break;
					}
				case VK_RIGHT:
					{
						m_ViewAngle += 0.1;
						break;
					}
				case VK_DOWN:
					{
						if(m_CameraPosition.y > -4.0)
							m_CameraPosition.y -= 0.1;
						break;
					}
				case VK_UP:
					{
						if(m_CameraPosition.y < 20.0)
							m_CameraPosition.y += 0.1;
						break;
					}
				case VK_SPACE:
					{
						static int temp = 100;
						gCoreMgr->HandleMessage( new SMessageRunScript( "shoot.txt" ) );
						gCoreMgr->HandleMessage( new SMessageAddEntity( &temp, EP_PLAYERMARBLE, m_CameraPosition ) );
						gCoreMgr->HandleMessage( new SMessageSetVelocity( &temp, (D3DXVECTOR3(0,0,0) - m_CameraPosition) * 1 ) );
						//Change state
						m_SubState = SGS_HOLD;
						break;
					}
				case VK_NUMPAD2:
					{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 0.0f, 10000.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, -500.0f, 0.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
					}
				case VK_NUMPAD3:
					{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, -10000.0f, 10000.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, -500.0f, 1000.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
					}
				case VK_NUMPAD1:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, -500.0f, -1000.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}
				case VK_NUMPAD5:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 0.0f, 10000.0f)));
					gCoreMgr->HandleMessage(new SMessageSetForce(&temp, D3DXVECTOR3(0.0f, 500.0f, 0.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}
				case VK_NUMPAD6:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 0.0f, 10000.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, 0.0f, 1000.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}
				case VK_NUMPAD4:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, 0.0f, -1000.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}
				case VK_NUMPAD8:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 80000.0f, 0.0f)));
					gCoreMgr->HandleMessage(new SMessageSetForce(&temp, D3DXVECTOR3(0.0f, 500.0f, 0.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}
				case VK_NUMPAD9:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 8000.0f, 0.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, 0.0f, 1000.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}
				case VK_NUMPAD7:
				{
					static int temp = 100;
					gCoreMgr->HandleMessage(new SMessageRunScript("shoot.txt"));
					gCoreMgr->HandleMessage(new SMessageAddEntity(&temp, EP_PLAYERMARBLE, m_CameraPosition));
					gCoreMgr->HandleMessage(new SMessageSetVelocity(&temp, (D3DXVECTOR3(0, 0, 0) - m_CameraPosition) * 1.5f));
					gCoreMgr->HandleMessage(new SMessageSetAngularVelocity(&temp, D3DXVECTOR3(0.0f, 8000.0f, 0.0f)));
					gCoreMgr->HandleMessage(new SMessageImpulseForce(&temp, D3DXVECTOR3(0.0f, 0.0f, -1000.0f)));
					//Change state
					m_SubState = SGS_HOLD;
					break;
				}

				}
				delete msg;
				break;
			}
		default:
			delete msg;
			break;
		}
		m_QueueBuffer.pop();
	}
}

void GameCore::ExecuteMessageAI()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_AISHOOT:
			{
				SMessageAIShoot* msgx = (SMessageAIShoot*)msg;
				static int temp = 100;
				gCoreMgr->HandleMessage( new SMessageRunScript( "shoot.txt" ) );
				gCoreMgr->HandleMessage( new SMessageAddEntity( &temp, EP_PLAYERMARBLE, m_CameraPosition ) );
				gCoreMgr->HandleMessage( new SMessageSetVelocity( &temp, (msgx->Target - m_CameraPosition) * 1 ) );
				m_SubState = SGS_HOLD;
				delete msg;
				break;
			}
		default:
			{
				delete msg;
				break;
			}
		}
		m_QueueBuffer.pop();
	}
}