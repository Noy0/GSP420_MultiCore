#include "SceneManager.h"

SceneManager::SceneManager(DD_ScreenTex** screentexOut, DD_ScreenText** screentextOut, DD_StaticMesh** staticmeshOut)
{
	*screentexOut = &m_ScreenTex;
	*screentextOut = (DD_ScreenText*)&m_ScreenText;
	*staticmeshOut = &m_StaticMesh;
	m_NextID = 1;
}

void SceneManager::Begin()
{
	//Copy External items here if necessary
}

void SceneManager::Update(float dt)
{
	//Update each element if necessary
}

void SceneManager::Clear()
{
	m_Items.clear();
}

int SceneManager::AddItem(int type, int *id)
{
	SceneObject newitem(type);
	pair<int, SceneObject> newset;
	newset.first = m_NextID;
	newset.second = newitem;
	if(id != 0)
		*id = m_NextID;
	m_Items.push_back(newset);
	return m_NextID++;
}

bool SceneManager::RemoveItem(int id)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == id)
		{
			m_Items.erase(m_Items.begin() + i);
			return true;
		}
	}
	return false;
}

void SceneManager::Export()
{
	vector<pair<int, D3DXMATRIX>> newScreenTex;
	vector<DD_ScreenTextData> newScreenText;
	vector<DD_StaticMeshData> newStaticMesh;
	
	newScreenTex.resize(m_Items.size());
	newScreenText.resize(m_Items.size());
	newStaticMesh.resize(m_Items.size());
	int ScreenTexCount = 0;
	int ScreenTextCount = 0;
	int StaticMeshCount = 0;

	for(int i = 0; i < m_Items.size(); ++i)
	{
		switch(m_Items[i].second.Type)
		{
		case SCENE_SCREENTEX:
			{
				newScreenTex[ScreenTexCount].first = m_Items[i].second.TextureID;
				newScreenTex[ScreenTexCount].second = m_Items[i].second.Transform;
				++ScreenTexCount;
				break;
			}
		case SCENE_SCREENTEXT:
			{
				newScreenText[ScreenTextCount].FontID = m_Items[i].second.FontID;
				newScreenText[ScreenTextCount].Text = m_Items[i].second.Text;
				newScreenText[ScreenTextCount].Color = m_Items[i].second.Color;
				newScreenText[ScreenTextCount].Transform = m_Items[i].second.Transform;
				++ScreenTextCount;
				break;
			}
		case SCENE_STATICMESH:
			{
				newStaticMesh[StaticMeshCount].MeshID = m_Items[i].second.MeshID;
				newStaticMesh[StaticMeshCount].TextureID = m_Items[i].second.TextureID;
				newStaticMesh[StaticMeshCount].Transform = m_Items[i].second.Transform;
				++StaticMeshCount;
				break;
			}
		}
	}

	newScreenTex.resize(ScreenTexCount);
	newScreenText.resize(ScreenTextCount);
	newStaticMesh.resize(StaticMeshCount);

	m_ScreenTex.Overwrite(newScreenTex);
	m_ScreenText.Overwrite(newScreenText);
	m_StaticMesh.Overwrite(newStaticMesh);
}

bool SceneManager::SwapTexture(int *ID, int TextureID)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *ID)
		{
			m_Items[i].second.TextureID = TextureID;
			return true;
		}
	}
	return false;
}

bool SceneManager::SwapMesh(int *ID, int MeshID)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *ID)
		{
			m_Items[i].second.MeshID = MeshID;
			return true;
		}
	}
	return false;
}

bool SceneManager::SetItemMatrix(int *ID, D3DXMATRIX Matrix)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *ID)
		{
			m_Items[i].second.Transform = Matrix;
			return true;
		}
	}

	return false;
}

bool SceneManager::SetItemText(int *ID, std::string str)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *ID)
		{
			m_Items[i].second.Text = str;
			return true;
		}
	}

	return false;
}

bool SceneManager::SetItemColor(int *ID, D3DXCOLOR color)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *ID)
		{
			m_Items[i].second.Color = color;
			return true;
		}
	}
}