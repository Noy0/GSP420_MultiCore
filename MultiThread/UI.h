#ifndef _UI_H
#define _UI_H

#include <d3dx9.h>
#include <windows.h>
#include <vector>
using std::vector;

/*
Each UI can react to following instructions (function call):
 - Check for mousing over
 - React to a click (loaction not distinguished
 - React to a keypress
A UI can either 'accept' or 'reject' an instruction.
By accepting an instruction the UI returns its ID.
By rejecting an instrction the UI returns 0.
*/

enum EUI
{
	UI_NULL,
	UI_BUTTON,
	UI_TEXTBOX,
	UI_WINDOW
};

struct SUIInfo
{
	int ID;
	EUI type;
	D3DXVECTOR3 Position;
	POINT Size;
	int TextureNormal;
	int TextureMouseOver;
	int TexturePressed;
	int Font;
	SUIInfo() :
	ID(0), Position(0,0,0), TextureNormal(0), TextureMouseOver(0),
	TexturePressed(0), Font(0)
	{
	}
};

class BaseUI
{
public:
	inline bool IsActivated(){return m_Activated;}
	virtual void Activate(bool active);
	virtual int GetType() = 0;
	virtual int GetID(){return m_ID;}
	virtual int GetTexture() = 0;
	virtual void Update(float dt) = 0;
	virtual int MouseOverCheck(POINT mousePos);
	virtual int Click(){return 0;}
	virtual int MouseButtonDown(int button){return 0;}
	virtual int MouseButtonUp(int button){return 0;}
	virtual int KeyPress(char key){return 0;}
	virtual virtual void SetPosition(D3DXVECTOR3 pos){m_Position = pos;}
	void SetSize(POINT size){m_Size = size;}
	D3DXVECTOR3 GetPosition(){return m_Position;}
	POINT GetSize(){return m_Size;}
	BaseUI()
	{
		m_MousedOver = 0;
		m_Activated = 0;
	}
	BaseUI(int id, D3DXVECTOR3 position, POINT size):
	m_ID(id), m_Position(position), m_Size(size)
	{
		m_MousedOver = false;
		m_Activated = false;
		m_MousePressed = false;
	}
	virtual ~BaseUI(){}
protected:
	virtual int MouseOver() = 0;
	virtual int MouseOut() = 0;

	D3DXVECTOR3 m_Position;
	POINT m_Size;
	bool m_MousedOver;
	bool m_MousePressed;
	bool m_Activated;
	int m_ID;
};

class Button : public BaseUI
{
public:
	Button(SUIInfo info);
	~Button();
	virtual int GetType(){return UI_BUTTON;}
	virtual void Update(float dt){}
	virtual int GetTexture();
	//virtual int MouseOverCheck(POINT mousePos);
	virtual int Click();
	virtual int MouseButtonDown(int button);
	virtual int MouseButtonUp(int button);
	//virtual int KeyPress(char key){}
private:
	virtual int MouseOver();
	virtual int MouseOut();

	int m_TextureNormal;
	int m_TextureMousedOver;
	int m_TexturePressed;
};

class TextBox : public BaseUI
{
public:
	TextBox(SUIInfo info);
	~TextBox();
	virtual int GetType(){return UI_TEXTBOX;}
	virtual void Update(float dt){}
	virtual int GetTexture();
	int GetFont(){return m_Font;}
	//virtual int MouseOverCheck(POINT mousePos);
	virtual int Click();
	virtual int MouseButtonDown(int button);
	virtual int MouseButtonUp(int button);
	virtual int KeyPress(char key);

	std::string GetText(){return m_Text;}
	bool IsTyping(){return m_Typable;}
private:
	virtual int MouseOver();
	virtual int MouseOut();

	int m_Texture;
	int m_Font;

	std::string m_Text;

	bool m_Typable;
};

class Window : public BaseUI
{
public:
	Window(SUIInfo info);
	~Window();
	virtual int GetType(){return UI_WINDOW;}
	virtual void Update(float dt);
	virtual int GetTexture();
	virtual int MouseOverCheck(POINT mousePos);
	virtual int Click();
	virtual int KeyPress(char key);
	virtual void SetPosition(D3DXVECTOR3 pos);
	virtual vector<BaseUI*>* GetSubUI(){return &m_UIList;}
private:
	virtual int MouseOver();
	virtual int MouseOut();

	vector<BaseUI*> m_UIList;
	int m_Texture;
};

#endif