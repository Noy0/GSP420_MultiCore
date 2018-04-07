#pragma once

#include ".\FuzzyLogic\Module\module.h"
#include "Message.h"
#include "DD_AIData\DDSTeeringObstacles.h"
#include "DD_AIData\DDSteering.h"

enum AI_MESSAGE
{
	//FUZZY LOGIC
	MSG_NULL1,
	MSG_ADDRULE,
	MSG_FUZZY,
	MSG_DEFUZZY,
	MSG_LASTMOVE,
	MSG_AITURN,

	//MOVEMENT
	
	MSG_REGISTERSTEERING,
	MSG_AIREGISTERPOSITION,


	//PATHFINDING
	MSG_CREATEGRAPH,
	MSG_CHANGESOURCE,
	MSG_CHANGETARGET,
	MSG_CREATEPATHASTAR,
	MSG_LOAD,
	MSG_SAVE,
	MSG_POINTTOINDEX,
	MSG_GETPATH,
	MSG_GETGRAPH



};

struct SAddRule : public SMessage
{
	SAddRule(ProxySet newAnt, ProxySet newCon):SMessage(CORE_AI,MSG_ADDRULE),Ant(newAnt),Con(newCon)
	{}

	ProxySet Ant;
	ProxySet Con;
		
};

struct SFuzzify : public SMessage
{
	SFuzzify(string name, double val):SMessage(CORE_AI,MSG_FUZZY),FLV(name),FuzzVal(val)

	{}
		
	string FLV;
	double FuzzVal;
};

struct SDefuzzify : public SMessage
{
	SDefuzzify(string name):SMessage(CORE_AI,MSG_DEFUZZY),DLV(name)
	
	{}

	string DLV;
};

struct SLastMove : public SMessage
{
	SLastMove():SMessage(CORE_AI,MSG_LASTMOVE)
	{}

};

struct SMessageRegisterSteering : public SMessage
{
	SMessageRegisterSteering(DD_SteeringData* STEERING,DD_SteeringObstacles* OBST)
							:SMessage(CORE_AI,MSG_REGISTERSTEERING), Steering(STEERING), Obst(OBST)
	{}
	DD_SteeringData* Steering;
	DD_SteeringObstacles* Obst;
};

struct SMessageRegisterAIPosition : public SMessage
{
	SMessageRegisterAIPosition(DD_Positions* AIPos)
							:SMessage(CORE_AI,MSG_AIREGISTERPOSITION), EntityPos(AIPos)

	{}

	DD_Positions* EntityPos;
	
};

struct SGetPath : public SMessage
{
	SGetPath():SMessage(CORE_AI, MSG_GETPATH) 
	{}
};

struct SGetGraph : public SMessage
{
	SGetGraph():SMessage(CORE_AI, MSG_GETGRAPH) 
	{}
};

struct SPointToIndex : public SMessage
{
	SPointToIndex(POINTS p, int& NodeIndex):SMessage(CORE_AI, MSG_POINTTOINDEX), pp(p), iNodeIndex(NodeIndex)
	{}
	POINTS pp;
	int&   iNodeIndex;
};

struct SSave : public SMessage
{
	SSave(char* FileName):SMessage(CORE_AI, MSG_SAVE), cFileName(FileName)
	{}
	char* cFileName;
};

struct SLoad : public SMessage
{
	SLoad(char* FileName):SMessage(CORE_AI, MSG_LOAD), cFileName(FileName)
	{}
	char* cFileName;
};

struct SCreatePathAStar : public SMessage
{
	SCreatePathAStar():SMessage(CORE_AI, MSG_CREATEPATHASTAR)
	{}
};

struct SChangeTarget : public SMessage
{
	SChangeTarget(int Cell):SMessage(CORE_AI, MSG_CHANGETARGET), iCell(Cell)
	{}
	int iCell;
};

struct SCreateGraph : public SMessage
{
	SCreateGraph(int CellsUp, int CellsAcross):SMessage(CORE_AI, MSG_CREATEGRAPH), iCellsUp(CellsUp), iCellsAcross(CellsAcross)
	{}
	int iCellsUp;
	int iCellsAcross;
};

struct SChangeSource : public SMessage
{
	SChangeSource(int Cell):SMessage(CORE_AI, MSG_CHANGESOURCE), iCell(Cell)
	{}
	int iCell;
};

struct SMessageAITurn : public SMessage
{
	SMessageAITurn(D3DXVECTOR3 Camera, D3DXVECTOR3 Target):SMessage(CORE_AI, MSG_AITURN), CameraPos(Camera), AI_Target(Target)

	{}

		D3DXVECTOR3 CameraPos;
		D3DXVECTOR3 AI_Target;

};



