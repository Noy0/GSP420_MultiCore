#ifndef _GAMEMESSAGE_H
#define _GAMEMESSAGE_H

#include "Message.h"
#include "DDPositions.h"

enum EGameMessage
{
	MSG_QUITGAME,
	MSG_CLICKEDUI,
	MSG_PICKED,
	MSG_RECEIVEDFROMNET,
	MSG_GAMEKEYDOWN,
	MSG_GAMEREGISTERPOSITION,
	MSG_AISHOOT
};

struct SMessageQuitGame : public SMessage
{
	SMessageQuitGame() : SMessage(CORE_GAME, MSG_QUITGAME)
	{}
};

struct SMessageClickedUI : public SMessage
{
	SMessageClickedUI(int ui_id) : SMessage(CORE_GAME, MSG_CLICKEDUI), UI_ID(ui_id)
	{}
	int UI_ID;
};

struct SMessagePicked : public SMessage
{
	SMessagePicked(int id) : SMessage(CORE_GAME, MSG_PICKED), ID(id)
	{}
	int ID;
};

struct SMessageReceivedFromNet : public SMessage
{
	SMessageReceivedFromNet(void* data, int size) : SMessage(CORE_GAME, MSG_RECEIVEDFROMNET),
		Data(data), Size(size)
	{}
	void* Data;
	int Size;
};

struct SMessageGameKeyDown : public SMessage
{
	SMessageGameKeyDown(int vk) : SMessage(CORE_GAME, MSG_GAMEKEYDOWN)
	{
		VK = vk;
	}
	int VK;
};

struct SMessageGameRegisterPosition : public SMessage
{
	SMessageGameRegisterPosition(DD_Positions* pos) : SMessage(CORE_GAME, MSG_GAMEREGISTERPOSITION)
	{
		pPositions = pos;
	}
	DD_Positions* pPositions;
};

struct SMessageAIShoot : public SMessage
{
	SMessageAIShoot(D3DXVECTOR3 target) : SMessage(CORE_GAME, MSG_AISHOOT)
	{
		Target = target;
	}
	D3DXVECTOR3 Target;
};

#endif