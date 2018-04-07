#include "UIManager.h"
#include "FontList.h"

UIManager::UIManager()
{
	m_NextID = 1;
}

UIManager::~UIManager()
{
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		delete m_UIList[i];
	}
}

int UIManager::AddUI(SUIInfo info)
{
	switch(info.type)
	{
	case UI_BUTTON:
		{
			m_UIList.push_back(new Button(info));
			break;
		}
	case UI_TEXTBOX:
		{
			m_UIList.push_back(new TextBox(info));
			break;
		}
	case UI_WINDOW:
		{
			m_UIList.push_back(new Window(info));
			break;
		}
	default:
		return 0;
	}
	return info.ID;
}

bool UIManager::RemoveUI(int id)
{
	return DeleteUI(&m_UIList, id);
}

void UIManager::Update(float dt)
{
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		m_UIList[i]->Update(dt);
	}
}

bool UIManager::ActivateUI(int id)
{
	return mActivateUI(&m_UIList, id, true);
}

bool UIManager::DisableUI(int id)
{
	return mActivateUI(&m_UIList, id, false);
}

void UIManager::Export(vector<pair<int, D3DXMATRIX>>* outputSceneTexture,
		vector<DD_ScreenTextData>* outputSceneText,
		vector<pair<int, std::string>>* outputText)
{
	mExportScene(&m_UIList, outputSceneTexture, outputSceneText);
	mExportText(&m_UIList, outputText);
}

int UIManager::MouseOverCheck(POINT mousePos)
{
	int result = 0;
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		if(result == 0)
			result = m_UIList[i]->MouseOverCheck(mousePos);
	}
	return result;
}

int UIManager::Click()
{
	int result = 0;
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		if(result == 0)
			result = m_UIList[i]->Click();
	}
	return result;
}

int UIManager::MouseButtonDown(int button)
{
	int result = 0;
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		if(result == 0)
			result = m_UIList[i]->MouseButtonDown(button);
	}
	return result;
}

int UIManager::MouseButtonUp(int button)
{
	int result = 0;
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		if(result == 0)
			result = m_UIList[i]->MouseButtonUp(button);
	}
	return result;
}

int UIManager::KeyPress(char key)
{
	int result = 0;
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		if(result == 0)
			result = m_UIList[i]->KeyPress(key);
	}
	return result;
}

bool UIManager::Typeing()
{
	return mTyping(&m_UIList);
}

void UIManager::SetPosition(int id, D3DXVECTOR3 position)
{
	BaseUI* temp = FindUI(&m_UIList, id);
	if(id == 0)
		return;
	temp->SetPosition(position);
}

void UIManager::SetSize(int id, POINT size)
{
	BaseUI* temp = FindUI(&m_UIList, id);
	if(id == 0)
		return;
	temp->SetSize(size);
}

bool UIManager::DeleteUI(vector<BaseUI*>* list, int id)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->GetID() == id)
		{
			delete (*list)[i];
			list->erase(list->begin() + i);
			return true;
		}
		if((*list)[i]->GetType() == UI_WINDOW)
		{
			bool result = DeleteUI(((Window*)((*list)[i]))->GetSubUI(), id);
			if(result)
				return true;
		}
	}
	return false;
}

bool UIManager::mActivateUI(vector<BaseUI*> *list, int id, bool activate)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->GetID() == id)
		{
			(*list)[i]->Activate(activate);
			return true;
		}
		if((*list)[i]->GetType() == UI_WINDOW)
		{
			bool result = mActivateUI(((Window*)((*list)[i]))->GetSubUI(), id, activate);
			if(result)
				return true;
		}
	}
	return false;
}

void UIManager::MoveUIToWindow(int id, int windowID)
{
	BaseUI* window = FindUI(&m_UIList, windowID);
	if(window->GetType() != UI_WINDOW)
		return;
	vector<BaseUI*>* loc = FindUIContainer(&m_UIList, id);
	if(loc == 0)
		return;
	int idx;
	for(int i = 0; i < loc->size(); ++i)
	{
		if((*loc)[i]->GetID() == id)
		{
			((Window*)window)->GetSubUI()->push_back((*loc)[i]);
			loc->erase(loc->begin() + i);
		}
	}
}

BaseUI* UIManager::FindUI(vector<BaseUI*> *list, int id)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->GetID() == id)
			return (*list)[i];
		if((*list)[i]->GetType() == UI_WINDOW)
		{
			BaseUI* temp = FindUI(((Window*)((*list)[i]))->GetSubUI(), id);
			if(temp!= 0) 
				return temp;
		}
	}
	return 0;
}

vector<BaseUI*>* UIManager::FindUIContainer(vector<BaseUI*> *list, int id)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->GetID() == id)
			return list;
		else if((*list)[i]->GetType() == UI_WINDOW)
		{
			vector<BaseUI*>* temp = FindUIContainer( ((Window*)((*list)[i]))->GetSubUI(), id);
			if(temp != 0)
				return temp;
		}
	}
	return 0;
}

void UIManager::mExportScene(vector<BaseUI*> *list, vector<pair<int,D3DXMATRIX> > *outputTexture, 
						vector<DD_ScreenTextData> *outputText)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->IsActivated())
		{
			//All UI has a texture, export that first
			pair<int, D3DXMATRIX> temp;
			temp.first = (*list)[i]->GetTexture();
			D3DXVECTOR3 pos = (*list)[i]->GetPosition();
			D3DXMatrixTranslation(&temp.second, pos.x, pos.y, pos.z);
			outputTexture->push_back(temp);
			switch((*list)[i]->GetType())
			{
			case UI_TEXTBOX:
				{
					DD_ScreenTextData textData;
					textData.Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					textData.FontID = FONT_CHAT;
					textData.Text = ((TextBox*)((*list)[i]))->GetText();
					textData.Transform = temp.second;
					outputText->push_back(textData);
					break;
				}
			case UI_WINDOW:
				{
					mExportScene(((Window*)((*list)[i]))->GetSubUI(), outputTexture, outputText);
					break;
				}
			}
		}
	}
}

void UIManager::mExportText(vector<BaseUI*> *list, vector<pair<int,std::string> > *outputText)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->GetType() == UI_TEXTBOX)
		{
			pair<int, std::string> temp;
			temp.first = (*list)[i]->GetID();
			temp.second = ((TextBox*)((*list)[i]))->GetText();
			outputText->push_back(temp);
		}
		else if((*list)[i]->GetType() == UI_WINDOW)
		{
			mExportText(((Window*)((*list)[i]))->GetSubUI(), outputText);
		}
	}
}

bool UIManager::mTyping(std::vector<BaseUI*> *list)
{
	for(int i = 0; i < list->size(); ++i)
	{
		if((*list)[i]->GetType() == UI_TEXTBOX)
		{
			if(((TextBox*)((*list)[i]))->IsTyping())
				return true;
		}
		else if ((*list)[i]->GetType() == UI_WINDOW)
		{
			if( mTyping(((Window*)((*list)[i]))->GetSubUI()))
				return true;
		}
	}
	return false;
}