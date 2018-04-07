#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include "ResourceManager.h"
#include "TextureList.h"

class TextureManager: public ResourceManager<IDirect3DTexture9*>
{
public:
	static void Initialize(IDirect3DDevice9* device);
	//void AddItem(int key, IDirect3DTexture9* item) -- same as default
	void AddItemFromFile(int key, char* filename);
	//IDirect3DTexture9* GetItem(int key); -- same as default
	void Clear();
	~TextureManager();
private:
	TextureManager();

	IDirect3DDevice9* m_pDevice;
};

extern TextureManager* gTextureMgr;

#endif