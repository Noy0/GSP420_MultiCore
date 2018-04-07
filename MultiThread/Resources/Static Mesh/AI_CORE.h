#ifndef _AICORE_H
#define _AICORE_H

#include "BaseCore.h"
#include ".\Movement\CMovement_Algorithms.h"
#include ".\Pathfinding\PathPlanner.h"
#include ".\FuzzyLogic\Module\module.h"
#include ".\DD_AIData\DDSTeeringObstacles.h"
#include ".\DD_AIData\DDSteering.h"

class AI_Core : public BaseCore
{
public:

	AI_Core();
	~AI_Core();

	void Update(float dt);

	void LoadFuzzy();

private:

	void AIMessage();


	//////////////FuzzyLogic//////////////
	Module m_Module;
	//Variables m_Variable;
	//ProxySet m_Set;
	

	/////////////PathFinding///////////////
	PathPlanner m_PathPlanner;
	

	/////////////Movement/////////////////
	//CMovement_Algorithms<entity,entity>* m_pSteering;
	DD_SteeringData*	DDSteeringData;
	DD_SteeringObstacles* DDSteeringObstacle;


};

#endif