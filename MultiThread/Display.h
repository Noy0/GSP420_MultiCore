#pragma once 
#include <windows.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <tchar.h>

using std::vector;

struct Light
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	D3DXVECTOR3 dirW;
};
class GraphicsEngine
{
public:
	GraphicsEngine(	HWND Hwnd );
	~GraphicsEngine();
	IDirect3DDevice9* GetDevice(){return mDevice;}
	void InitDirectX();
	void RenderState();
	void BuildFX();
	void PreRender();
	void CreateMatrix();
	void WireFrame();
	void PostRender();
	void ViewMatrix();
	void RenderTexture(IDirect3DTexture9* Tex, D3DXMATRIX Combine);
	void RenderMesh(ID3DXMesh* Mesh,  vector<IDirect3DTexture9*> m_textures, vector<D3DMATERIAL9> m_materials, D3DXMATRIX Trans);
	void RenderMeshTex(ID3DXMesh* Mesh, IDirect3DTexture9* m_textures, vector<D3DMATERIAL9> m_materials, D3DXMATRIX Trans);
	void RenderFont(ID3DXFont * mFont, std::string Text,D3DXCOLOR color, D3DXMATRIX Trans);
	void CleanUp();
	void ChangeBGColor(int R, int G, int B);
	void ChangeUP(D3DXVECTOR3 UP);
	void ChangePos(D3DXVECTOR3 Pos);
	void ChangeLook(D3DXVECTOR3 Look);
	void RotationCamera(float Angle);
	D3DXVECTOR3 GetCameraPos(){return mCameraPos;}
	D3DXVECTOR3 GetCameraTarget(){return mCameraUp;}
	D3DXVECTOR3 GetCameraUp(){return mCameraUp;}
	D3DXMATRIX GetProjection(){return proj;}
	D3DXMATRIX GetViewMatrix(){return view;}
	int GetScreenWidth(){return width;}
	int GetScreenHeight(){return height;}

	ID3DXSprite* GetSprite(){return mSprite;}
private:
	 
	IDirect3DDevice9* mDevice;
	ID3DXSprite     * mSprite;
	ID3DXEffect     * mEffect;
	HWND			  mHwnd;
	HINSTANCE         mInst;
	D3DMATERIAL9      m_white;
	D3DPRESENT_PARAMETERS d3dpp;
	IDirect3DTexture9* m_WhiteTex;
	int				 width;
	int				 height;
	bool			 windowed;	
	bool			 wireFrame;

	D3DXHANDLE		m_hTech;
	D3DXHANDLE		m_hWVP;
	D3DXHANDLE		m_hWorldInvTrans;
	D3DXHANDLE		m_hEyePos;
	D3DXHANDLE		m_hWorld;
	D3DXHANDLE		m_hTex;
	D3DXHANDLE		m_hMtrl;
	D3DXHANDLE		m_hLight;

	Light           mLight;
	D3DXVECTOR3     mCameraUp;
	D3DXVECTOR3     mCameraTarget;
	D3DXVECTOR3     mCameraPos;

	float m_FOV;
	float m_NearPlane;
	float m_FarPlane;

	//Temp
	D3DXMATRIX view, proj, world, identity;
	int R, G, B;
};