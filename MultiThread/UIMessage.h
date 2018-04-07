#ifndef _UIMESSAGE_H
#define _UIMESSAGE_H

#include "Message.h"
#include "UI.h"
#include <windows.h>
#include "DDCamera.h"
#include "DDPicking.h"

enum EUIMessage
{
	MSG_ADDUI,
	MSG_REMOVEUI,
	MSG_ACTIVATEUI,
	MSG_DISABLEUI,
	MSG_WINDOW,
	MSG_UIREGISTERCAMERA,
	MSG_REGISTERPICKING
};

struct SMessageAddUI : public SMessage
{
	SMessageAddUI(SUIInfo info) : SMessage(CORE_UI, MSG_ADDUI),
		Info(info)
	{}
	~SMessageAddUI(){}
	SUIInfo Info;
};

struct SMessageRemoveUI : public SMessage
{
	SMessageRemoveUI(int id) : SMessage(CORE_UI, MSG_REMOVEUI),
		ID(id)
	{}
	~SMessageRemoveUI(){}
	int ID;
};

struct SMessageActivateUI : public SMessage
{
	SMessageActivateUI(int id) : SMessage(CORE_UI, MSG_ACTIVATEUI),
		ID(id)
	{}
	~SMessageActivateUI(){}
	int ID;
};

struct SMessageDisableUI : public SMessage
{
	SMessageDisableUI(int id) : SMessage(CORE_UI, MSG_DISABLEUI),
		ID(id)
	{}
	~SMessageDisableUI(){}
	int ID;
};

struct SMessageWindow : public SMessage
{
	SMessageWindow(MSG msg) : SMessage(CORE_UI, MSG_WINDOW),
		Msg(msg)
	{}
	MSG Msg;
};

struct SMessageUIRegisterCamera : public SMessage
{
	SMessageUIRegisterCamera(DD_Camera* camera) : SMessage(CORE_UI, MSG_UIREGISTERCAMERA), Camera(camera)
	{}
	DD_Camera* Camera;
};

struct SMessageRegisterPicking : public SMessage
{
	SMessageRegisterPicking(DD_Picking* picking) : SMessage(CORE_UI, MSG_REGISTERPICKING), Picking(picking)
	{}
	DD_Picking* Picking;
};

#endif