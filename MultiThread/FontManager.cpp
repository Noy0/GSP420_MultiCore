#include "FontManager.h"

FontManager* gFontMgr = 0;

void FontManager::Initialize(IDirect3DDevice9 *device)
{
	static FontManager instance;
	instance.m_pDevice = device;
	gFontMgr = &instance;
}

void FontManager::AddItem(int key, D3DXFONT_DESC desc)
{
	ID3DXFont* newFont = 0;
	D3DXCreateFontIndirect(m_pDevice, &desc, &newFont);
	m_Element.push_back(pair<int, ID3DXFont*>(key, newFont));
}

void FontManager::Clear()
{
	for(int i = 0; i < m_Element.size(); ++i)
	{
		if(m_Element[i].second)
			m_Element[i].second->Release();
	}
	m_Element.clear();
}

FontManager::~FontManager()
{
	Clear();
}