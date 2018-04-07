#ifndef _DDSTATICMESH_H
#define _DDSTATICMESH_H

#include "DynamicData.h"
#include <d3dx9.h>

struct DD_StaticMeshData
{
	int MeshID;
	int TextureID;
	D3DXMATRIX Transform;
};

class DD_StaticMesh : public DynamicData<vector<DD_StaticMeshData>>
{
public:
	DD_StaticMesh();
private:
};

#endif