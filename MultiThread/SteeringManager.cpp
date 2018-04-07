#include "SteeringManager.h"

SteeringConstantsMgr* gSCMgr;

void SteeringConstantsMgr::Initialize()
{
	static SteeringConstantsMgr instance;
	gSCMgr = &instance;
}

void SteeringConstantsMgr::AddItem(int key, SteeringConstants *item)
{
	pair<int, SteeringConstants*> temp;
	temp.second = new SteeringConstants;
	*(temp.second) =*item;
	temp.first = key;
	m_Element.push_back(temp);
}