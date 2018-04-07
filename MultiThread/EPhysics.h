#ifndef _EPHYSICS_H
#define _EPHYSICS_H

//Editing done by - TP, DN   (to make it easier to understand, and allow new collider types)

enum TypeCollider
{
	COLLIDER_NULL,
	COLLIDER_PLANE,
	COLLIDER_BOX,
	COLLIDER_SPHERE,
	COLLIDER_CAPSULE,
	COLLIDER_CYLINDER,
};

enum TypePhysics
{
	PHYSICS_NULL, 
	PHYSICS_STATIC,
	PHYSICS_DYNAMIC,
	PHYSICS_KINEMATIC
};

#endif