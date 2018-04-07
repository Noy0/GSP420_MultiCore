#include "TextureManager.h"

//TextureManager
TextureManager* gTextureMgr = 0;

TextureManager::TextureManager()
{
}

void TextureManager::Initialize(IDirect3DDevice9 *device)
{
	static TextureManager instance;
	instance.m_pDevice = device;
	gTextureMgr = &instance;
}

void TextureManager::AddItemFromFile(int key, char *filename)
{
	IDirect3DTexture9* newTex;
	D3DXCreateTextureFromFile(m_pDevice, (LPCSTR)filename, &newTex);
	pair<int, IDirect3DTexture9*> newPair(key, newTex);
	m_Element.push_back(newPair);
}

void TextureManager::Clear()
{
	for(unsigned i = 0; i < m_Element.size(); ++i)
	{
		m_Element[i].second->Release();
	}

	m_Element.clear();
}

TextureManager::~TextureManager()
{
	Clear();
}