#ifndef _STATICMESHMANAGER_H
#define _STATICMESHMANAGER_H

#include "ResourceManager.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "StaticMeshList.h"


struct SMeshData
{
	ID3DXMesh* MeshSys;
	vector<IDirect3DTexture9*> Texture;
	vector<D3DMATERIAL9> Material;
	~SMeshData();
};

class StaticMeshManager : public ResourceManager<SMeshData*>
{
public:
	static void Initailize(IDirect3DDevice9* device);
	void AddItem(int key, SMeshData* item);
	void AddItemFromFile(int key, char* filename);
	SMeshData* GetItem(int key);
	//SMeshData* GetItemPtr(int key);
	void Clear();
	~StaticMeshManager();
private:
	StaticMeshManager();

	IDirect3DDevice9* m_pDevice;

	//Vertex struct
	struct VertexPNT
	{
		VertexPNT()
			:pos(0.0f, 0.0f, 0.0f),
			normal(0.0f, 0.0f, 0.0f),
			tex0(0.0f, 0.0f){}
		VertexPNT(float x, float y, float z, 
			float nx, float ny, float nz,
			float u, float v):pos(x,y,z), normal(nx,ny,nz), tex0(u,v){}
		VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
			:pos(v),normal(n), tex0(uv){}

		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 tex0;

		static IDirect3DVertexDeclaration9* Decl;
	};
};

extern StaticMeshManager* gStaticMeshMgr;

#endif