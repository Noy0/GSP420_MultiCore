#ifndef _DDSCREENTEX_H
#define _DDSCREENTEX_H

#include "DynamicData.h"
#include <utility>
#include <d3dx9.h>

class DD_ScreenTex : public DynamicData<vector<pair<int, D3DXMATRIX>>>
{
public:
	//static DD_ScreenTex* Instance();
	DD_ScreenTex();
private:
};

#endif