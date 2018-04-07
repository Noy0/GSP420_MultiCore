#ifndef _GRAPHICSENGINE_H
#define _GRAPHICSENGINE_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

//Using this enum user can input technique to be used in shader
//within the message
enum ETechnique
{
};

class GraphicsEngine
{
public:
	//Core functions
	GraphicsEngine(HWND hwnd);
	//bool HandleMessage(SMessage* pMessage);
	//void Update(float dt);
	~GraphicsEngine();

	//Class specific public functions
	IDirect3DDevice9* GetDevice(){return m_pDevice;}
	void Begin();
	void DrawTexture(IDirect3DTexture9* texture, D3DXMATRIX* transform);
	void Draw_Text(ID3DXFont* font, std::string text, D3DXMATRIX* transform, D3DXCOLOR color);
	void End();
	//void DrawStaticMesh(int key, D3DXMATRIX* transform);
private:

	//Basic members for graphics
	//HINSTANCE m_hAppInst;
	HWND m_hWnd;
	IDirect3D9* m_pD3D;
	D3DPRESENT_PARAMETERS m_D3DPP;
	IDirect3DDevice9* m_pDevice;

	//Class specific private functions
	//void ExecuteMessage();
	void BuildMatrix();

	//Additional member for functionalities
	ID3DXSprite* m_pSprite;

	//Light stuff
	struct Light
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR spec;
		D3DXVECTOR3 dirW;
	};

	Light m_Light;

	//Shader stuff
	
	ID3DXEffect* m_pFX;
	D3DXHANDLE   m_hTech;
	D3DXHANDLE   m_hWVP;
	D3DXHANDLE   m_hWorldInvTrans;
	D3DXHANDLE   m_hEyePos;
	D3DXHANDLE   m_hWorld;
	D3DXHANDLE   m_hTex;
	D3DXHANDLE   m_hMtrl;
	D3DXHANDLE   m_hLight;

	//Camera stuff
	D3DXVECTOR3 m_CameraPos;
	D3DXVECTOR3 m_CameraTarget;
	D3DXVECTOR3 m_CameraUp;
	D3DXMATRIX m_View;

	float m_FOV;
	float m_NearPlane;
	float m_FarPlane;
	D3DXMATRIX m_Projection;
};

#endif