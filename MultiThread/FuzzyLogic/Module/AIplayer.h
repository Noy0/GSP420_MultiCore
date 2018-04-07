#pragma once
#include "..\..\Movement\SVector3D.h"
#include <d3dx9.h>

#include "module.h"


class Player_AI
{ 
public:
		
	Player_AI();
	void LoadAISet();
	int GetDesirability(int,bool,bool);
	bool IsInFOV(D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3);
	double GetDistance(D3DXVECTOR3);
	bool OnLine(D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3);
	
private:
	
	int DesireValue;
	SVector3D origin;
	double FOV;
	bool InFOV;
	SVector3D AI_CamPos;
	SVector3D AI_Facing;
	SVector3D AI_MarblePos;
	double slope;

	Module g_Module;
};


