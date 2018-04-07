#ifndef _PHYSICSMESSAGE_H
#define _PHYSICSMESSAGE_H

#include "..//Message.h"

enum EPhysicsMessage
{
	MSG_NULL,
	MSG_SETVELOCITY,
	MSG_ADDFORCE,
	MSG_CREATEBOX,
	MSG_CREATESPHERE,
	MSG_CREATECYLINDER,
	MSG_SETROTATION,
	MSG_ADDROTATIONONX,
	MSG_ADDROTATIONONY,
	MSG_ADDROTATIONONZ,
	MSG_GETOBJPROPERTIES,
	MSG_SETASKINAMATIC,
	MSG_SETVELOCITYX,
	MSG_SETVELOCITYY,
	MSG_SETVELOCITYZ,
	MSG_ADDFORCEX,
	MSG_ADDFORCEY,
	MSG_ADDFORCEZ

};

struct SMessageSetVelocity : public SMessage
{
	SMessageSetVelocity(int ID, D3DXVECTOR3 velocity):SMessage(CORE_PHYSICS, MSG_SETVELOCITY)
	{
		mID = ID;
		mVelocity = velocity;
	}
	int mID;
	D3DXVECTOR3 mVelocity;
};
struct SMessageAddForce : public SMessage
{
	SMessageAddForce(int ID,D3DXVECTOR3 force) : SMessage(CORE_PHYSICS, MSG_ADDFORCE)
	{
		mID = ID;
		mForce = force;
	}
	int mID;
	D3DXVECTOR3 mForce;
};
struct SMessageCreateBox : public SMessage
{
	SMessageCreateBox(float size, D3DXVECTOR3 origin, float mass):SMessage(CORE_PHYSICS, MSG_CREATEBOX)
	{

		mHalfScale.x = size/2;
		mHalfScale.y = size/2;
		mHalfScale.z = size/2;
		mOrigin = origin;
		mMass = mass;
	}
	D3DXVECTOR3 mHalfScale;
	D3DXVECTOR3 mOrigin;
	float mMass;
};
struct SMessageCreateSphere : public SMessage
{
	SMessageCreateSphere(float radius,D3DXVECTOR3 origin, float mass):SMessage(CORE_PHYSICS, MSG_CREATESPHERE)
	{
		mRadius = radius;
		mOrigin = origin;
		mMass = mass;
	}
	float mRadius;
	D3DXVECTOR3 mOrigin;
	float mMass;
	//SMessageCreateObject():// in work
};
struct SMessageCreateCylinder : public SMessage
{
	SMessageCreateCylinder(D3DXVECTOR3 size, D3DXVECTOR3 origin, float mass):SMessage(CORE_PHYSICS, MSG_CREATECYLINDER)
	{
		mHalfScale.x = size.x/2;
		mHalfScale.y = size.y/2;
		mHalfScale.z = size.z/2;
		mOrigin = origin;
		mMass = mass;
	}
	D3DXVECTOR3 mHalfScale;
	D3DXVECTOR3 mOrigin;
	float mMass;
	//SMessageCreateObject():(CORE_PHYSICS, MSG_CREATEOBJECT)// in work
};
struct SMessageSetRotation : public SMessage
{
	SMessageSetRotation(int ID, D3DXVECTOR3 axis, float degree):SMessage(CORE_PHYSICS, MSG_SETROTATION)
	{
		mID = ID;
		mAxis = axis;
		mDegree = degree;
	}
	int mID;
	D3DXVECTOR3 mAxis;
	float mDegree;
};
struct SMessageAddRotationOnX : public SMessage
{
	SMessageAddRotationOnX(int ID, float degree):SMessage(CORE_PHYSICS, MSG_ADDROTATIONONX)
	{
		mID = ID;
		mDegree = degree;
	}
	int mID;
	float mDegree;
	
};
struct SMessageAddRotationOnY : public SMessage
{
	SMessageAddRotationOnY(int ID, float degree):SMessage(CORE_PHYSICS, MSG_ADDROTATIONONY)
	{
		mID = ID;
		mDegree = degree;
	}
	int mID;
	float mDegree;
};
struct SMessageAddRotationOnZ : public SMessage
{
	SMessageAddRotationOnZ(int ID, float degree):SMessage(CORE_PHYSICS, MSG_ADDROTATIONONZ)
	{
		mID = ID;
		mDegree = degree;
	}
	int mID;
	float mDegree;
};
struct SMessageGetObjectProperties : public SMessage
{
	SMessageGetObjectProperties(int ID):SMessage(CORE_PHYSICS, MSG_GETOBJPROPERTIES)
	{
		mID = ID;
	}
	int mID;
};

struct SMessageSetAsKinamatic : public SMessage
{
	SMessageSetAsKinamatic(int id):SMessage(CORE_PHYSICS, MSG_SETASKINAMATIC)
	{
		mID = id;
	}
	int mID;
};

struct SMessageSetVelocityX : public SMessage
{
	SMessageSetVelocityX(int id, float velocity):SMessage(CORE_PHYSICS,MSG_SETVELOCITYX)
	{
		mID = id;
		mVel = velocity;
	}
	int mID;
	int mVel;
};

struct SMessageSetVelocityY : public SMessage
{
	SMessageSetVelocityY(int id, float velocity):SMessage(CORE_PHYSICS,MSG_SETVELOCITYY)
	{
		mID = id;
		mVel = velocity;
	}
	int mID;
	int mVel;
};

struct SMessageSetVelocityZ : public SMessage
{
	SMessageSetVelocityZ(int id, float velocity):SMessage(CORE_PHYSICS,MSG_SETVELOCITYZ)
	{
		mID = id;
		mVel = velocity;
	}
	int mID;
	int mVel;
};



#endif