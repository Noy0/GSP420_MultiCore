#include "EntityManager.h"
#include "EntityPropertyManager.h"

//Heavily Edited done by - TP  (to fix seriously crappy implemntation of a list manager)


EntityData::EntityData(): position(0, 0, 0), velocity(0, 0, 0), force(0, 0, 0), rotation(0, 0, 0, 1),
						  physicsID(-1), steeringType(0), scriptFlag1(0), scriptFlag2(0), mat(&defaultMat) {}



//EntityData methods - DN
PhysicsMat EntityData::getPMaterial()
{
	return *mat;
}

TypePhysics EntityData::getIsKinematic()
{
	return isKinematic;
}



D3DXVECTOR3 EntityData::getPosition()
{
	return position;
}

D3DXVECTOR3 EntityData::getVelocity()
{
	return velocity;
}

D3DXVECTOR3 EntityData::getAcceleration()
{
	return (force / mat->mass);
}

D3DXVECTOR3 EntityData::getMomentum()
{
	return (velocity * mat->mass);
}

D3DXVECTOR3 EntityData::getForce()
{
	return force;
}



D3DXQUATERNION EntityData::getRotation()
{
	return rotation;
}

D3DXVECTOR3 EntityData::getAngularVelocity()
{
	return angularVelocity;
}

D3DXVECTOR3 EntityData::getAngularAcceleration()
{
	D3DXVECTOR3 angularAcceleration;
	D3DXVec3TransformNormal(&angularAcceleration, &torque, getInvTensorMatrix(mat->inertiaTensor));
	return angularAcceleration;
}

D3DXVECTOR3 EntityData::getAngularMomentum()
{
	D3DXVECTOR3 angularMomentum;
	D3DXVec3TransformNormal(&angularMomentum, &angularVelocity, getTensorMatrix(mat->inertiaTensor));
	return angularMomentum;
}

D3DXVECTOR3 EntityData::getTorque()
{
	return this->torque;
}

int EntityData::getSteeringType()
{
	return this->steeringType;
}



void EntityData::setPhysicsMat(PhysicsMat* mat)  
{
	this->mat = mat;
}

void EntityData::setIsKinematic(TypePhysics mode)
{
	this->isKinematic = mode;
}



void EntityData::setConstraintsLinear(bool x, bool y, bool z)
{
	constraintLinear.x = x;
	constraintLinear.y = y;
	constraintLinear.z = z;
}

void EntityData::setConstraintsAngular(bool x, bool y, bool z)
{
	constraintAngular.x = x;
	constraintAngular.y = y;
	constraintAngular.z = z;
}



void EntityData::setPosition(D3DXVECTOR3 position)
{
	this->position = position;
}

void EntityData::setVelocity(D3DXVECTOR3 velocity)
{
	this->velocity = velocity;
}

void EntityData::setForce(D3DXVECTOR3 force)
{
	this->force = force;
}



void EntityData::setRotation(D3DXQUATERNION rotation)
{
	this->rotation = rotation;
}

void EntityData::setAngularVelocity(D3DXVECTOR3 angularVelocity)
{
	this->angularVelocity = angularVelocity;
}

void EntityData::setTorque(D3DXVECTOR3 torque)
{
	this->torque = torque;
}

void EntityData::setSteeringType(int steering)
{
	this->steeringType = steering;
}



void EntityData::translate(D3DXVECTOR3 translation)
{
	this->position += translation;
}

void EntityData::accelerate(D3DXVECTOR3 deltaVelocity)
{
	this->velocity += deltaVelocity;
	
}

void EntityData::applyImpulseForce(D3DXVECTOR3 deltaMomentum)
{
	this->velocity += deltaMomentum/mat->mass;
}

void EntityData::applyForce(D3DXVECTOR3 force)
{
	this->force += force;
}



void EntityData::rotate(D3DXQUATERNION rotation)
{
	this->rotation = rotation * this->rotation;
}

void EntityData::applyAngularVelocity(D3DXVECTOR3 deltaAngularVelocity)
{
	this->angularVelocity += deltaAngularVelocity;
}

void EntityData::applyImpulseRotation(D3DXVECTOR3 deltaAngularMomentum)
{
	D3DXVECTOR3 deltaAngularVelocity;
	D3DXVec3TransformNormal(&deltaAngularVelocity, &deltaAngularMomentum, getInvTensorMatrix(mat->inertiaTensor));
	this->angularVelocity += deltaAngularVelocity;
}

void EntityData::applyTorque(D3DXVECTOR3 deltaTorque)
{
	this->torque += deltaTorque;
}



D3DXMATRIX* EntityData::getTensorMatrix(D3DXVECTOR3 inertiaTensor)
{
	return new D3DXMATRIX(inertiaTensor.x, 0, 0, 0,
						  0, inertiaTensor.y, 0, 0,
						  0, 0, inertiaTensor.z, 0,
						  0, 0, 0, 1);
}

D3DXMATRIX* EntityData::getInvTensorMatrix(D3DXVECTOR3 inertiaTensor)
{
	D3DXMATRIX* result = new D3DXMATRIX();
	D3DXMatrixInverse(result, NULL, getTensorMatrix(mat->inertiaTensor));
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


EntityManager::EntityManager()
{
	nextID = 0;
}

EntityManager::~EntityManager()
{
	entities.clear();
}



EntityData* EntityManager::NewEntity()
{
	EntityData temp;
	temp.ID = nextID++;
	entities.push_back(temp);
	return &entities.back();
}

void EntityManager::AddEntity(EntityData& entity)
{
	entities.push_back(entity);
}

bool EntityManager::RemoveEntity(int id)
{
	for (EntityIterator itr = entities.begin(); itr != entities.end(); ++itr)
	{
		if (itr->ID == id)
		{
			int physicsID = itr->physicsID;
			itr = entities.erase(itr);

			int highest = physicsID;
			EntityData* entry = nullptr;
			for (itr = entities.begin(); itr != entities.end(); ++itr)
			{
				if (itr->physicsID > highest)
				{
					highest = itr->physicsID;
					entry = &*itr;
				}
			}
			if (entry != nullptr)
			{
				entry->physicsID = physicsID;
			}
			return true;
		}
	}
	return false;
}

void EntityManager::Clear()
{
	entities.clear();
}



EntityData* EntityManager::GetEntity(int id)
{
	for (EntityIterator itr = entities.begin(); itr != entities.end(); ++itr)
	{
		if (itr->ID == id)
		{
			return &(*itr);
		}
	}
	return nullptr;
}



void EntityManager::CloneInto(EntityList& entityList)
{
	entityList = entities;
}

EntityIterator EntityManager::Begin()
{
	return entities.begin();
}

EntityIterator EntityManager::End()
{
	return entities.end();
}