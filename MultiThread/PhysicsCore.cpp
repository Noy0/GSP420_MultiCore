#include "PhysicsCore.h"
#include "Physics.h"
#include "PhysicsMessage.h"

PhysicsCore::PhysicsCore()
{
}

PhysicsCore::~PhysicsCore()
{

}

void PhysicsCore::OnLaunch()
{
	//make world
	WorldInfo winfo;
	gPhysics.CreateWorld(winfo);
}

void PhysicsCore::Update(float dt)
{
	//Copy msg from msgQ to buffer
	this->CopyMessage();
	//gPhysics->UPDATE
	gPhysics.UpdateWorld(dt,1);

}

void PhysicsCore::PhysicsMessage()
{
	

	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		
		case MSG_NULL:
			{
				break;
			}
		case MSG_SETVELOCITY:
			{
				SMessageSetVelocity* msgex = (SMessageSetVelocity*)(msg); 
				gPhysics.SetLinearVelocity(msgex->mID, gPhysics.ConvertToBtVec(msgex->mVelocity));
				delete msg;
				break;
			}

		case MSG_ADDFORCE:
			{
				SMessageAddForce* msgex = (SMessageAddForce*)(msg); 
				gPhysics.ApplyCentralForce(msgex->mID, gPhysics.ConvertToBtVec(msgex->mForce));
				delete msg;
				break;
			}

		case MSG_CREATEBOX:
			{
				SMessageCreateBox* msgex = (SMessageCreateBox*)(msg); 
				BoxInfo binfo;
				binfo.mass = msgex->mMass;
				binfo.orgin = gPhysics.ConvertToBtVec(msgex->mOrigin);
				binfo.halfscale = gPhysics.ConvertToBtVec(msgex->mHalfScale);
				gPhysics.CreateCollisionBox(binfo);
				delete msg;
				break;
			}

		case MSG_CREATESPHERE:
			{
				SMessageCreateSphere* msgex = (SMessageCreateSphere*)(msg); 
				SphereInfo sinfo;
				sinfo.radius = msgex->mRadius;
				sinfo.mass = msgex->mMass;
				sinfo.orgin = gPhysics.ConvertToBtVec(msgex->mOrigin);
				gPhysics.CreateCollisionSphere(sinfo);
				delete msg;
				break;
			}

		case MSG_CREATECYLINDER:
			{
				SMessageCreateCylinder* msgex = (SMessageCreateCylinder*)(msg); 
				BoxInfo cinfo;
				cinfo.halfscale = gPhysics.ConvertToBtVec(msgex->mHalfScale);
				cinfo.mass = msgex->mMass;
				cinfo.orgin = gPhysics.ConvertToBtVec(msgex->mOrigin);
				gPhysics.CreateCollisionCylinder(cinfo);
				delete msg;
				break;
			}

		case MSG_SETROTATION:
			{
				SMessageSetRotation* msgex = (SMessageSetRotation*)(msg); 

				gPhysics.SetShapeRotation(msgex->mID,gPhysics.ConvertToBtVec(msgex->mAxis),msgex->mDegree);
				delete msg;
				break;
			}

		case MSG_ADDROTATIONONX:
			{
				SMessageAddRotationOnX* msgex = (SMessageAddRotationOnX*)(msg); 
				gPhysics.RotateShapeOnX(msgex->mID, msgex->mDegree);
				delete msg;
				break;
			}

		case MSG_ADDROTATIONONY:
			{
				SMessageAddRotationOnY* msgex = (SMessageAddRotationOnY*)(msg);
				gPhysics.RotateShapeOnY(msgex->mID, msgex->mDegree);
				delete msg;
				break;
			}

		case MSG_ADDROTATIONONZ:
			{
				SMessageAddRotationOnZ* msgex = (SMessageAddRotationOnZ*)(msg);
				gPhysics.RotateShapeOnZ(msgex->mID, msgex->mDegree);
				delete msg;
				break;
			}

		case MSG_GETOBJPROPERTIES:
			{
				SMessageGetObjectProperties* msgex = (SMessageGetObjectProperties*)(msg);
				delete msg;
				break;
			}
		case MSG_SETASKINAMATIC:
			{
				SMessageSetAsKinamatic* msgex = (SMessageSetAsKinamatic*)(msg);
				gPhysics.SetAsKinematic(msgex->mID);
				delete msg;
				break;
			}
			
		default:
			{
				//temp.push(m_QueueBuffer.front());
				break;
			}
		}
		m_QueueBuffer.pop();
	}
}