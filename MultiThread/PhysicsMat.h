#ifndef _PHYSICSMAT_H
#define _PHYSICSMAT_H

//allows definition of common behavior states, for entities in the physics world - DN, TP   (replaces old inferior version)

#include "EPhysics.h"
#include <d3dx9.h>


struct PhysicsMat
{
	PhysicsMat() : inertiaTensor(0, 0, 0), scalar(1.0, 1.0, 1.0), friction(1), restitution(0.0), linearDamping(0),
		angularDamping(0.99), mass(1), type(COLLIDER_NULL) {}
	TypeCollider type;
	float mass;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	D3DXVECTOR3 inertiaTensor;
	D3DXVECTOR3 scalar;//(HALF SCALE) used to deform model
};

static PhysicsMat defaultMat;

struct PlanePMat :PhysicsMat
{
	PlanePMat() : PhysicsMat(), pNormal(0, 1.0, 0, 0) { type = COLLIDER_PLANE; }
	D3DXVECTOR4 pNormal;
};

struct BoxPMat :PhysicsMat
{
	BoxPMat() : PhysicsMat() { type = COLLIDER_BOX; }
};

struct SpherePMat :PhysicsMat
{
	SpherePMat() : PhysicsMat(), radius(1) { type = COLLIDER_SPHERE; }
	float radius;
};

struct CapsulePMat :PhysicsMat
{
	CapsulePMat() : PhysicsMat(), radius(1), length(1) { type = COLLIDER_CAPSULE; }
	float radius;
	float length;
};

struct CylinderPMat :PhysicsMat
{
	CylinderPMat() : PhysicsMat(), radius(1), length(1) { type = COLLIDER_CYLINDER; }
	float radius;
	float length;
};

#endif