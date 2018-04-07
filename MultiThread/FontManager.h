#ifndef _FONTMANAGER_H
#define _FONTMANAGER_H

#include "ResourceManager.h"
#include <d3d9.h>
#include <d3dx9.h>

class FontManager : public ResourceManager<ID3DXFont*>
{
public:
	static void Initialize(IDirect3DDevice9* device);
	void AddItem(int key, D3DXFONT_DESC desc);
	void Clear();
	~FontManager();
private:
	FontManager(){}

	IDirect3DDevice9* m_pDevice;
};

extern FontManager* gFontMgr;

#endif