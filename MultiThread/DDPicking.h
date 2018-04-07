#ifndef _DDPICKING_H
#define _DDPICKING_H

#include "DynamicData.h"
#include <vector>
using std::vector;
#include <d3dx9.h>

struct DDPickingData
{
	vector<int> ID;
	vector<D3DXVECTOR3> Position;
	vector<D3DXVECTOR3> Halfscale;
};

class DD_Picking : public DynamicData<DDPickingData>
{
public:
	DD_Picking() : DynamicData("TimWuRules_Picking")
	{}
};

#endif