#ifndef _UICORE_H
#define _UICORE_H

#include "BaseCore.h"
#include "UIManager.h"
#include "Picking.h"
#include "DDCamera.h"
#include "DDPicking.h"

class UICore : public BaseCore
{
public:
	UICore(HWND hwnd);
	//bool HandleMessage(SMessage* message);
	~UICore();
private:
	void OnLaunch();
	void Update(float dt);
	void UIMessage();

	HWND m_hWnd;

	UIManager m_UIManager;

	POINT m_MousePos;

	//Picking
	bool m_EnablePicking;
	DD_Camera* m_Camera;
	DD_Picking* m_DDPicking;
	Picking m_Picking;

	DD_ScreenTex m_SceneTextureOut;
	DD_ScreenText m_SceneTextOut;

};

#endif