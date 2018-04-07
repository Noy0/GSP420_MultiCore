#ifndef _UIMANAGER_H
#define _UIMANAGER_H

#include "UI.h"
#include <vector>
#include "DDScreenTex.h"
#include "DDScreenText.h"
#include <string>

class UIManager
{
public:
	UIManager();
	//UIManager(DD_ScreenTex* ddTex, DD_ScreenText* ddText);
	~UIManager();
	int AddUI(SUIInfo info);
	bool RemoveUI(int id);
	bool ActivateUI(int id);
	bool DisableUI(int id);
	void MoveUIToWindow(int id, int windowID);
	void Update(float dt);
	void Export(vector<pair<int, D3DXMATRIX>>* outputSceneTexture,
		vector<DD_ScreenTextData>* outputSceneText,
		vector<pair<int, std::string>>* outputText);
	int MouseOverCheck(POINT mousePos);
	int Click();
	int MouseButtonDown(int button);
	int MouseButtonUp(int button);
	int KeyPress(char key);
	bool Typeing();
	void SetPosition(int id, D3DXVECTOR3 position);
	void SetSize(int id, POINT size);
private:
	BaseUI* FindUI(vector<BaseUI*> *list, int id);
	bool DeleteUI(vector<BaseUI*>* list, int id);
	bool mActivateUI(vector<BaseUI*>* list, int id, bool activate);
	vector<BaseUI*>* FindUIContainer(vector<BaseUI*> *list, int id);
	void mExportScene(vector<BaseUI*>* list, vector<pair<int, D3DXMATRIX>>* outputTexture,
		vector<DD_ScreenTextData>* outputText);
	void mExportText(vector<BaseUI*> *list,
		vector<pair<int, std::string>>* outputText);
	bool mTyping(vector<BaseUI*> *list);

	vector<BaseUI*> m_UIList;
	int m_NextID;
};

#endif