#include "EntityPropertyManager.h"

EntityPropertyMgr* gEPMgr;

void EntityPropertyMgr::Initialize()
{
	static EntityPropertyMgr instance;
	gEPMgr = &instance;
}

void EntityPropertyMgr::AddItem(int key, EntityPropertyData *item)
{
	pair<int, EntityPropertyData*> temp;
	temp.second = new EntityPropertyData;
	*(temp.second) =*item;
	temp.first = key;
	m_Element.push_back(temp);
}