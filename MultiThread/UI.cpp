#include "UI.h"

void BaseUI::Activate(bool active)
{
	m_Activated = active;
	if(active == false)
	{
		m_MousedOver = false;
		m_MousePressed = false;
	}
}

int BaseUI::MouseOverCheck(POINT mousePos)
{
	if(!m_Activated)
		return 0;
	if(mousePos.x < m_Position.x || mousePos.y < m_Position.y ||
		mousePos.x > m_Position.x + m_Size.x || mousePos.y > m_Position.y + m_Size.y)
	{
		return MouseOut();
	}
	return MouseOver();
}

Button::Button(SUIInfo info):
BaseUI(info.ID, info.Position, info.Size),
m_TextureNormal(info.TextureNormal), m_TextureMousedOver(info.TextureMouseOver),
m_TexturePressed(info.TexturePressed)
{
	if(m_TextureMousedOver == 0)
		m_TextureMousedOver = m_TextureNormal;
	if(m_TexturePressed == 0)
		m_TexturePressed = m_TextureMousedOver;
}

Button::~Button()
{
}

int Button::GetTexture()
{
	if(m_MousedOver)
		if(m_MousePressed)
			return m_TexturePressed;
		else
			return m_TextureMousedOver;
	else
		return m_TextureNormal;
}

int Button::Click()
{
	if(!m_Activated || !m_MousedOver)
		return 0;
	return m_ID;
}

int Button::MouseButtonDown(int button)
{
	if(!m_Activated || !m_MousedOver)
		return 0;
	switch(button)
	{
	case 0:
		{
			m_MousePressed = true;
			return m_ID;
			break;
		}
	}
	return 0;
}

int Button::MouseButtonUp(int button)
{
	if(!m_Activated)
		return 0;
	if(m_MousedOver && m_MousePressed)
	{
		m_MousePressed = false;
		return m_ID;
	}
	m_MousePressed = false;
	return 0;
}

int Button::MouseOver()
{
	if(m_Activated)
		m_MousedOver = true;
	return 0;
}

int Button::MouseOut()
{
	if(m_Activated)
		m_MousedOver = false;
	return 0;
}

TextBox::TextBox(SUIInfo info):
BaseUI(info.ID, info.Position, info.Size),
m_Texture(info.TextureNormal), m_Font(info.Font), m_Typable(false)
{
}

TextBox::~TextBox()
{
}

int TextBox::GetTexture()
{
	return m_Texture;
}

int TextBox::Click()
{
	if(m_Activated)
		if(m_MousedOver)
			m_Typable = true;
		else
			m_Typable = false;
	return 0;
}

int TextBox::MouseButtonDown(int button)
{
	if(!m_Activated)
		return 0;
	if(!m_MousedOver)
		m_Typable = false;
	switch(button)
	{
	case 0:
		{
			m_MousePressed = true;
			return 0;
			break;
		}
	}
	return 0;
}

int TextBox::MouseButtonUp(int button)
{
	if(!m_Activated)
		return 0;
	if(m_MousedOver && m_MousePressed)
	{
		m_MousePressed = false;
		m_Typable = true;
		return m_ID;
	}
	m_MousePressed = false;
	return 0;
}

int TextBox::KeyPress(char key)
{
	if(!m_Activated)
		return 0;
	if(m_Typable)
	{
		switch(key)
		{
		case 8:
			{
				if(m_Text.size())
					m_Text.resize(m_Text.length() - 1);
				break;
			}
		case 13:
			{
				m_Text += '\n'; 
				//return m_ID;
				break;
			}
		default:
			{
				if(key >= 32 && key <= 126)
					m_Text += key;
			}
		}
	}
	return 0;
}

int TextBox::MouseOver()
{
	if(m_Activated)
		m_MousedOver = true;
	return 0;		
}

int TextBox::MouseOut()
{
	if(m_Activated)
		m_MousedOver = false;
	return 0;
}

Window::Window(SUIInfo info):
BaseUI(info.ID, info.Position, info.Size),
m_Texture(info.TextureNormal)//, m_SceneTextureID(info.SceneTextID)
{
}

Window::~Window()
{
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		delete m_UIList[i];
	}
}

void Window::Update(float dt)
{
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		m_UIList[i]->Update(dt);
	}
}

int Window::GetTexture()
{
	return m_Texture;
}

int Window::MouseOverCheck(POINT mousePos)
{
	if(!m_Activated)
		return 0;
	if(mousePos.x < m_Position.x || mousePos.y < m_Position.y ||
		mousePos.x > m_Position.x + m_Size.x || mousePos.y > m_Position.y + m_Size.y)
	{
		return MouseOut();
	}
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		m_UIList[i]->MouseOverCheck(mousePos);
	}
	return MouseOver();
}

int Window::Click()
{
	if(m_MousedOver)
	{
		int firstvalue = 0;
		for(int i = 0; i < m_UIList.size(); ++i)
		{
			if(firstvalue == 0)
				firstvalue = m_UIList[i]->Click();
			else
				m_UIList[i]->Click();
		}
		return firstvalue;
	}
	return 0;
}

int Window::KeyPress(char key)
{
	if(m_MousedOver)
	{
		int firstvalue = 0;
		for(int i = 0; i < m_UIList.size(); ++i)
		{
			if(firstvalue == 0)
				firstvalue = m_UIList[i]->KeyPress(key);
			else
				m_UIList[i]->KeyPress(key);
		}
		return firstvalue;
	}
	return 0;
}

void Window::SetPosition(D3DXVECTOR3 pos)
{
	m_Position = pos;
	for(int i = 0; i < m_UIList.size(); ++i)
	{
		m_UIList[i]->SetPosition(pos + m_Position);
	}
}

int Window::MouseOver()
{
	return 0;
}

int Window::MouseOut()
{
	return 0;
}