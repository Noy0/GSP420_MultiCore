#include "StaticMeshManager.h"

StaticMeshManager* gStaticMeshMgr = 0;

IDirect3DVertexDeclaration9* StaticMeshManager::VertexPNT::Decl = 0;

StaticMeshManager::StaticMeshManager()
{
}

SMeshData::~SMeshData()
{
	if(MeshSys)
		MeshSys->Release();
	for(int i = 0; i < Texture.size(); ++i)
	{
		if(Texture[i])
			Texture[i]->Release();
	}
}

void StaticMeshManager::Initailize(IDirect3DDevice9 *device)
{
	static StaticMeshManager instance;
	instance.m_pDevice = device;

	//Initilize vertex declaration
	
	D3DVERTEXELEMENT9 VertexPNTElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};	

	instance.m_pDevice->CreateVertexDeclaration(VertexPNTElements, &VertexPNT::Decl);

	gStaticMeshMgr = &instance;
}

void StaticMeshManager::AddItem(int key, SMeshData* item)
{
}

void StaticMeshManager::AddItemFromFile(int key, char *filename)
{
	SMeshData* newMesh = new SMeshData();
	ID3DXMesh* mesh = 0;
	ID3DXBuffer* adjBuffer = 0; //In case of needing optimizing
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	D3DXLoadMeshFromX((LPCSTR)filename, D3DXMESH_MANAGED, m_pDevice, &adjBuffer, 
		&mtrlBuffer, 0, &numMtrls, &mesh);

	if(!mesh)
		return;

	//Find out of there's normal info
	D3DVERTEXELEMENT9 element[MAX_FVF_DECL_SIZE];

	mesh->GetDeclaration(element);
	bool normal = false;
	for(int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if(element[i].Stream = 0xff)
			break;
		if(element[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normal = true;
			break;
		}
	}

	//convert mesh to desired vertex format here
	UINT numElem = 0;
	VertexPNT::Decl->GetDeclaration(element, &numElem);
	mesh->CloneMesh(D3DXMESH_SYSTEMMEM, element, m_pDevice, &(newMesh->MeshSys));
	if(mesh)
		mesh->Release();

	if(normal == false)
	{
		D3DXComputeNormals(newMesh->MeshSys, 0);
	}
	//Optimize mesh

	newMesh->MeshSys->Optimize(D3DXMESH_MANAGED | D3DXMESHOPT_COMPACT | 
		D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)(adjBuffer->GetBufferPointer()), 0, 0, 0, &mesh);
	newMesh->MeshSys->Release();
	newMesh->MeshSys = mesh;
	if(adjBuffer)
		adjBuffer->Release();

	//Materials
	if(mtrlBuffer && numMtrls)
	{
		D3DXMATERIAL* xmtrl = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for(DWORD i = 0; i < numMtrls; ++i)
		{
			D3DMATERIAL9 mtrl;
			mtrl.Ambient = xmtrl[i].MatD3D.Diffuse;
			mtrl.Diffuse = xmtrl[i].MatD3D.Diffuse;
			mtrl.Emissive = xmtrl[i].MatD3D.Emissive;
			mtrl.Specular = xmtrl[i].MatD3D.Specular;
			mtrl.Power = xmtrl[i].MatD3D.Power;
			newMesh->Material.push_back(mtrl);
			
			if(xmtrl[i].pTextureFilename == 0)
			{
				newMesh->Texture.push_back(0);
			}
			else
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(m_pDevice, (LPCSTR)xmtrl[i].pTextureFilename,
					&tex);
				newMesh->Texture.push_back(tex);
			}
		}
	}

	mtrlBuffer->Release();

	//Add the pair in
	pair<int, SMeshData*> newPair(key, newMesh);
	m_Element.push_back(newPair);
}

SMeshData* StaticMeshManager::GetItem(int key)
{
	return ResourceManager::GetItem(key);
}

void StaticMeshManager::Clear()
{
	ResourceManager::Clear();
}

StaticMeshManager::~StaticMeshManager()
{
	Clear();
}