#ifndef _UTIL_H
#define _UTIL_H

#include "btBulletDynamicsCommon.h"

//Editing done by - TP   (removed old inferior version of PhysicsMat)

struct WorldInfo
{
	WorldInfo(): Gravity(0,-10,0),WorldAabbMin(-10000,-10000,-10000),
		WorldAabbMax(10000,10000,10000),MaxProxies(1024){}
	btVector3 Gravity;
	btVector3 WorldAabbMin;
	btVector3 WorldAabbMax;
	int MaxProxies;
};

#endif