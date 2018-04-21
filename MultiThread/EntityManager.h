#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <List>
#include "PhysicsMat.h"

//Heavily Edited done by - TP  (to fix seriously crappy implemntation of a list manager)

struct TriBool
{
	bool x, y, z;
};

struct EntityData
{
public:
	int ID;
	int physicsID;
	int type;

	//deprecated to match UML -DN
	////Physics attribures
	//D3DXVECTOR3 position;
	//D3DXVECTOR3 velocity;
	//D3DXVECTOR3 force;
	//D3DXQUATERNION rotation;
	//end deprecation

//private:
	//additions made for GSP420 -DN TP
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 force;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 angularVelocity;
	D3DXVECTOR3 torque;
	PhysicsMat* mat;
	TypePhysics isKinematic;
	TriBool constraintLinear;
	TriBool constraintAngular;

	D3DXMATRIX* getTensorMatrix(D3DXVECTOR3 inertiaTensor);
	D3DXMATRIX* getInvTensorMatrix(D3DXVECTOR3 inertiaTensor);

public:
	//methods
	PhysicsMat getPMaterial();
	TypePhysics getIsKinematic();

	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getVelocity();
	D3DXVECTOR3 getAcceleration();
	D3DXVECTOR3 getMomentum();
	D3DXVECTOR3 getForce();

	D3DXQUATERNION getRotation();
	D3DXVECTOR3 getAngularVelocity();
	D3DXVECTOR3 getAngularAcceleration();
	D3DXVECTOR3 getAngularMomentum();
	D3DXVECTOR3 getTorque();
	
	int getSteeringType();

	void setPhysicsMat(PhysicsMat* mat);
	void setIsKinematic(TypePhysics mode);

	void setConstraintsLinear(bool x, bool y, bool z);
	void setConstraintsAngular(bool x, bool y, bool z);

	void setPosition(D3DXVECTOR3 position);
	void setVelocity(D3DXVECTOR3 velocity);
	void setForce(D3DXVECTOR3 force);

	void setRotation(D3DXQUATERNION rotation);
	void setAngularVelocity(D3DXVECTOR3 angularVelocity);
	void setTorque(D3DXVECTOR3 torque);

	void setSteeringType(int steeringType);

	void translate(D3DXVECTOR3 translation);
	void accelerate(D3DXVECTOR3 deltaVelocity);
	void applyImpulseForce(D3DXVECTOR3 deltaMomentum);
	void applyForce(D3DXVECTOR3 force);

	void rotate(D3DXQUATERNION rotation);
	void applyAngularVelocity(D3DXVECTOR3 deltaAngularVelocity);
	void applyImpulseRotation(D3DXVECTOR3 deltaAngularMomentum);
	void applyTorque(D3DXVECTOR3 deltaTorque);

	//end additions -DN TP


	//Graphics attribures

	//Scripting attributes
	int scriptFlag1;
	int scriptFlag2;

	//AI attributes
		//Steering
	int steeringType;

	EntityData();
};

typedef std::list<EntityData> EntityList;
typedef EntityList::iterator EntityIterator;
class EntityManager
{
	EntityList entities;
	int nextID;

public:
	EntityManager();
	~EntityManager();

	EntityData* NewEntity();
	void AddEntity(EntityData& entity);
	bool RemoveEntity(int id);
	void Clear();

	EntityData* GetEntity(int id);

	void CloneInto(EntityList& entityList);
	EntityIterator Begin();
	EntityIterator End();
};

#endif
