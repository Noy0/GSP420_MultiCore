#pragma once

#include "ResourceManager.h"
#include <d3dx9.h>
#include <string>
#include "EScene.h"
#include "EPhysics.h"
#include "PhysicsMat.h"

// Modified by TP (to enable PhysicsMats)

struct EntityPropertyData
{
	//Graphics
	int RenderType;
	int GResourceID;
	D3DXVECTOR3 GraphicsScale;

	//Physics
	int PhysicsType;
	PhysicsMat* mat;

	//AI
	//AI STATE INFO
	//The routine
	std::string ScriptName;

	EntityPropertyData()
	{
		
		//Set all types to 0
		RenderType = 0;
		GResourceID = 0;
		PhysicsType = 0;
		mat = &defaultMat;
	}
};

class EntityPropertyMgr : public ResourceManager<EntityPropertyData*>
{
public:
	static void Initialize();
	void AddItem(int key, EntityPropertyData* item);
private:
	EntityPropertyMgr(){}
};

extern EntityPropertyMgr* gEPMgr;