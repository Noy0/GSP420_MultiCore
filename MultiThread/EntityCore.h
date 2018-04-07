#ifndef _ENTITYCORE_H
#define _ENTITYCORE_H

#include "BaseCore.h"
#include "EntityManager.h"
#include "DDScriptData.h"
#include "DDStaticMesh.h"
#include "Physics.h"
#include "DDPicking.h"
#include "DDPositions.h"
#include ".\DD_AIData\DDSTeeringObstacles.h"
#include ".\DD_AIData\DDSteering.h"

/*
EntityCore - core that operate entities.
because of the mechanics of physics, it is currently combined with physics.
*/

class EntityCore : public BaseCore
{
public:
	EntityCore();
	void Update(float dt);

private:
	void OnLaunch();
	void EntityMgrMsg();
	void PhysicsMsg();
	void Export();

	void PollPhysics();


	//Entity manager
	EntityManager m_EntityMgr;
	//Physics engine
	PhysicsWorld m_Physics;
	//Exports
		//Graphics
	void TranslateToStaticMesh(EntityData &entity, DD_StaticMeshData &out);
	DD_StaticMesh m_StaticMeshExport;
		//Scripting
	DD_ScriptData m_ScriptExport;
	void TranslateToScriptData(EntityData &entity, ScriptData &out);
		//UI
	DD_Picking m_PickingExport;
	//void TranslateToPickingData(
		//Game
	DD_Positions m_PositionExport;
		//AI Steering
	DD_SteeringData	m_SteeringExport;
	DD_SteeringObstacles m_ObstacleExport;
	
};

#endif