#ifndef PHYSICS_CORE_H
#define PHYSICS_CORE_H

#include "..\\BaseCore.h"
#include "Physics.h"

class PhysicsCore : public BaseCore
{
public:
	PhysicsCore();
	~PhysicsCore();

private:
	void OnLaunch();
	void Update(float dt);
	void PhysicsMessage();

	//define physics (gPhysics)
	PhysicsWorld gPhysics;

};


#endif