#ifndef PICKING_H
#define PICKING_H

#include <d3dx9.h>
#include <d3d9.h>
#include <dxerr.h>
#include <vector>
//#include "OfficialPhysicsProject2\CMesh.h"
using namespace std;


struct PickingCheck
{
	D3DXVECTOR3 objectPos;
	int objectID;
};


class Picking
{
public:
	//Picking();
	//~Picking();
	int picking(POINT mousePos, D3DXMATRIX proj, D3DXMATRIX view, vector<int> id, vector<D3DXVECTOR3> pos, vector<D3DXVECTOR3> halfscale, float screenW, float screenH);

private:
	void getWorldPickingRay(POINT mousePos, D3DXVECTOR3* originW, D3DXVECTOR3* dirW, D3DXMATRIX proj, D3DXMATRIX view,float screenW, float screenH);
	
	int isPicked(D3DXVECTOR3 objectPos, D3DXVECTOR3 halfscale,D3DXVECTOR3* originW, D3DXVECTOR3* dirW);
	int distanceCheck(D3DXVECTOR3* originW);
	D3DXVECTOR3 mMinPt;
	D3DXVECTOR3 mMaxPt;
	vector<PickingCheck> mCheck;
	
};

#endif