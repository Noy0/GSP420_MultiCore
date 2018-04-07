#pragma once

#include "BaseCore.h"
#include "CoreManager.h"
#include "GraphicsMessage.h"
#include "EntityMessage.h"
#include "Movement\CMovement_Algorithms.h"
#include "Pathfinding\PathPlanner.h"
#include "DDCamera.h"
#include "Pathfinding\CNode.h"
//#include "FuzzyLogic\Module\module.h"
//#include "DD_AIData\ddsteeringobstacles.h"
//#include "DD_AIData\DDSteering.h"
#include "FuzzyLogic\Module\AIplayer.h"
#include "DDPositions.h"
class AI_Core : public BaseCore
{
public:

	AI_Core();
	~AI_Core();

	void Update(float dt);
	void LastMove();
	void AI_Turn(D3DXVECTOR3,D3DXVECTOR3);
	//void CheckDesireRate(int);
	int Desire;
	void LoadGraph();
	void SetPath();
	
	//int CurrentDesire;



private:

	void AIMessage();


	//////////////FuzzyLogic//////////////
	Module m_Module;
	Player_AI m_AI;
	double CurrentMarbleDist;
	int TopDesireEntity;
	int CurrentDesire;
	bool InFOV;
	bool BallBlock;
	bool ObjBlock;
	D3DXVECTOR3 TopDesire;
	D3DXVECTOR3 AI_Camera;
	vector<int> AI_ID;
	DD_Positions* PositionData;

		

	/////////////PathFinding///////////////
	PathPlanner m_pPathPlanner;
	//vector<CNode*>		m_vecNodes;
	vector<int>			v_Nodes;
	vector<int>			v_TargetNodes;
	int					m_NumNodes;
	
	int					m_TargetNode;
	list<int>				m_CurPath;
	int					PathIndex;
	int					ObjInd;
	int m_CurNode;
	

	/////////////Movement/////////////////
	CMovement_Algorithms* m_pSteering;
	//DD_SteeringData*	DDSteeringData;
	DD_SteeringData*    mDDSteeringData;
	DD_SteeringObstacles* DDSteeringObstacle;
	vector<pair<int,SVector3D>> m_vFinalSteering;
	




};

