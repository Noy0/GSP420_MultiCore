#ifndef _DDSCREENTEXT_H
#define _DDSCREENTEXT_H

#include "DynamicData.h"
#include <utility>
#include <d3dx9.h>
#include <string>

struct DD_ScreenTextData
{
	int FontID;
	std::string Text;
	D3DXMATRIX Transform;
	D3DXCOLOR Color;
};

class DD_ScreenText : public DynamicData<vector<DD_ScreenTextData>>
{
public:
	DD_ScreenText();
private:
};

#endif