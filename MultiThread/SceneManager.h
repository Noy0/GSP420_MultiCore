#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

/*
Interface for dynamic data used for graphics engine
Singleton
Includes all needed DD classes:
- Static Mesh
- Animated Mesh
- Dynamic 3D items (water, land)
- Texture, object space
- Texture, screen space
- Font, object space
- Font, screen space
*/

#include "BaseManager.h"
#include "DDScreenTex.h"
#include "DDScreenText.h"
#include "DDStaticMesh.h"
#include "EScene.h"
using std::vector;
using std::pair;

struct SceneObject
{
	int Type;
	int TextureID;
	int MeshID;
	int FontID;
	std::string Text;
	D3DXCOLOR Color;
	D3DXMATRIX Transform;
	SceneObject(){}
	SceneObject(int type) : Type(type)
	{
		TextureID = 0;
		MeshID = 0;
		FontID = 0;
		D3DXMatrixIdentity(&Transform);
	}
	SceneObject(int type, int texture, int mesh, int font, D3DXMATRIX transform):
	Type(type), TextureID(texture), MeshID(mesh), FontID(font), Transform(transform)
	{}
};

class SceneManager : public BaseManager<SceneObject>
{
public:
	SceneManager(DD_ScreenTex** screentexOut, DD_ScreenText** screentextOut, DD_StaticMesh** staticmeshOut);
	void Begin();
	void Update(float dt);
	void Clear();
	int AddItem(int type, int *id);
	bool RemoveItem(int id);
	virtual void Export();
	bool SwapTexture(int *ID, int TextureID);
	bool SwapMesh(int *ID, int MeshID);
	bool SetItemMatrix(int *ID, D3DXMATRIX Matrix);
	bool SetItemText(int *ID, std::string str);
	bool SetItemColor(int *ID, D3DXCOLOR color);
private:
	int m_NextID;
	DD_ScreenTex	m_ScreenTex;
	DD_ScreenText	m_ScreenText;
	DD_StaticMesh	m_StaticMesh;
};

extern SceneManager* gSceneMgr;

#endif