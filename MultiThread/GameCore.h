#ifndef _GAMECORE_H
#define _GAMECORE_H

#include "BaseCore.h"
#include "DDScreenText.h"
#include "DDScreenTex.h"
#include "DDStaticMesh.h"
#include "DDPositions.h"
#include "Networking\PacketPresets.h"

enum ENetDataType
{
	NDT_NULL = 3,
	NDT_UPDATESCENE
};

struct SGameNetData
{
	PacketHeader Header;
	int ID;
	vector<pair<int, D3DXVECTOR3>> Items;
};

class GameCore : public BaseCore
{
public:
	GameCore();
	~GameCore();
private:

	void OnLaunch();
	void Update(float dt);

	void ExecuteMessage();
	void ClickedUI(int id);
	void Picked(int id);
	void KeyDown(int vk);

	//Game States
	void MenuUpdate(float dt);
	void SingleUpdate(float dt);
	void HostingUpdate(float dt);
	void ClientUpdate(float dt);
		//Game Sub state message execution
	void ExecuteMessageMenu();
	void ExecuteMessageShoot();
	void ExecuteMessageNoAccess();
	void ExecuteMessageAI();

	//Game stuff here
	vector<DD_ScreenTextData> m_TextOutBuffer;
	DD_ScreenText m_TextOut;
	DD_ScreenTex m_TexOut;
	DD_StaticMesh m_StaticMeshOut;
	D3DXVECTOR3 m_CameraPosition;
	D3DXVECTOR3 LookAt;
	float m_ViewAngle;

	int m_PlayerScore[4];
	int m_Turn;
	int m_BallCount;
	bool m_GameOver = false;

	DD_ScreenTextData mPlayerData;

	int m_GameState;
	int m_SubState;

	DD_Positions* m_PositionIn;

	bool m_Hosting;
};

#endif