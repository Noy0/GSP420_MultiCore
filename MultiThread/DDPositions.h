#ifndef _DDPOSITIONS_H
#define _DDPOSITIONS_H

#include <d3dx9.h>
#include "DynamicData.h"

struct DDGameData
{
	int ID;
	int Type;
	D3DXVECTOR3 Position;
};

class DD_Positions : public DynamicData<vector<DDGameData>>
{
public:
	DD_Positions() : DynamicData("TimwuRules_Position"){}
};

#endif