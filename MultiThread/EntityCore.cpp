#include "EntityCore.h"
#include "EntityPropertyManager.h"
#include "EntityPropertyList.h"
#include "EntityMessage.h"
#include "GraphicsMessage.h"
#include "ScriptingCore\ScriptMessages.h"
#include "CoreManager.h"
#include "PhysicsMat.h"
#include "EPhysics.h"
#include "UIMessage.h"
#include "GameMessage.h"
#include "AI_Message.h"


//Editing done by - TP

EntityCore::EntityCore()
{
	WorldInfo winfo;
	m_Physics.CreateWorld(winfo);
}

void EntityCore::Update(float dt)
{
	CopyMessage();
	EntityMgrMsg();
	PhysicsMsg();
	m_Physics.UpdateWorld(dt, 1);
	PollPhysics();
	Export();
}

void EntityCore::OnLaunch()
{
	//Register graphics export
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_STATICMESH, &m_StaticMeshExport));
	//Register AI export
	gCoreMgr->HandleMessage(new SMessageRegisterScriptObject(&m_ScriptExport));
	//Register scripting export
	//Register UI export
	gCoreMgr->HandleMessage(new SMessageRegisterPicking(&m_PickingExport));

	//Register AI Steering
	gCoreMgr->HandleMessage(new SMessageRegisterSteering(&m_SteeringExport,&m_ObstacleExport));

	//Register Game export
	gCoreMgr->HandleMessage(new SMessageGameRegisterPosition(&m_PositionExport));
	gCoreMgr->HandleMessage(new SMessageRegisterAIPosition(&m_PositionExport));
}

void EntityCore::EntityMgrMsg()
{
	queue<SMessage*> tempQueue;
	while(!m_QueueBuffer.empty())
	{
		SMessage* p_msg = m_QueueBuffer.front();
		switch(p_msg->Message)
		{
		case MSG_ADDENTITY:
			{
				SMessageAddEntity* p_msgx = (SMessageAddEntity*)p_msg;
				EntityPropertyData* p_newEPD = gEPMgr->GetItem(p_msgx->Type);
				//Add entity to manager
				EntityData* p_entity = m_EntityMgr.NewEntity();
				*(p_msgx->ID) = p_entity->ID;
				p_entity->type = p_msgx->Type;
				p_entity->setPosition(p_msgx->Position);

				//default value set to wander for the marbles
				p_entity->setSteeringType(0);

				
				if(p_newEPD->mat != nullptr)
					p_entity->setPhysicsMat(p_newEPD->mat);

				//Add object to physics
				int pID = m_Physics.CreatePhysics_Object(*(p_newEPD->mat), p_msgx->Position);
				p_entity->physicsID = pID;
				m_Physics.SetIsKinematic(pID, (TypePhysics)p_newEPD->PhysicsType);

				delete p_msg;
				break;
			}
		case MSG_REMOVEENTITY:
			{
				SMessageRemoveEntity* msgx = (SMessageRemoveEntity*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				//Remove object from physics
				if(p_entity->physicsID > -1)
					m_Physics.DeletePhysicsObject(p_entity->physicsID);
				//Remove entity from manager
				m_EntityMgr.RemoveEntity(eID);

				delete p_msg;
				break;
			}
		case MSG_SETPHYSICSMATERIAL:
			{
				SMessageSetMaterial* msgx = (SMessageSetMaterial*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setPhysicsMat(msgx->P_PMat);

				if (p_entity->physicsID > -1)
					m_Physics.setPhysicsMat(p_entity->physicsID, *msgx->P_PMat);

				delete p_msg;
				break;
			}
		case MSG_SETISKINEMATIC:
			{
				SMessageSetIsKinematic* msgx = (SMessageSetIsKinematic*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setIsKinematic(msgx->Type);

				if (p_entity->physicsID > -1)
					m_Physics.SetIsKinematic(eID, msgx->Type);

				delete p_msg;
				break;
			}
		case MSG_SETCONSTRAINTLINEAR:
			{
				SMessageSetConstraintLinear* msgx = (SMessageSetConstraintLinear*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setConstraintsLinear(msgx->X, msgx->Y, msgx->Z);

				if (p_entity->physicsID > -1)
					m_Physics.SetConstraintsLinear(eID, msgx->X, msgx->Y, msgx->Z);

				delete p_msg;
				break;
			}
		case MSG_SETCONSTRAINTANGULAR:
			{
				SMessageSetConstraintAngular* msgx = (SMessageSetConstraintAngular*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setConstraintsAngular(msgx->X, msgx->Y, msgx->Z);

				if (p_entity->physicsID > -1)
					m_Physics.SetConstraintsAngular(eID, msgx->X, msgx->Y, msgx->Z);

				delete p_msg;
				break;
			}
		case MSG_SETPOSITION:
			{
				SMessageSetPosition* msgx = (SMessageSetPosition*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setPosition(msgx->Position);

				if (p_entity->physicsID > -1)
					m_Physics.SetPosition(p_entity->physicsID, msgx->Position);

				delete p_msg;
				break;
			}
		case MSG_SETVELOCITY:
			{
				SMessageSetVelocity* msgx = (SMessageSetVelocity*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setVelocity(msgx->Velocity);

				if (p_entity->physicsID > -1)
					m_Physics.SetLinearVelocity(p_entity->physicsID, msgx->Velocity);

				delete p_msg;
				break;
			}
		case MSG_SETFORCE:
			{
				SMessageSetForce* msgx = (SMessageSetForce*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setForce(msgx->Force);

				if (p_entity->physicsID > -1)
					m_Physics.SetCentralForce(p_entity->physicsID, msgx->Force);

				delete p_msg;
				break;
			}
		case MSG_SETROTATION:
			{
				SMessageSetRotation* msgx = (SMessageSetRotation*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if (p_entity->physicsID > -1)
					m_Physics.SetRotation(p_entity->physicsID, msgx->Axis, msgx->Degree);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->setRotation(Rot);

				delete p_msg;
				break;
			}
		case MSG_SETROTATION_Q:
			{
				SMessageSetRotation_Quat* msgx = (SMessageSetRotation_Quat*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setRotation(msgx->Rotation);

				if (p_entity->physicsID > -1)
					m_Physics.SetRotation(p_entity->physicsID, msgx->Rotation);

				delete p_msg;
				break;
			}
		case MSG_SETANGULARVELOCITY:
			{
				SMessageSetAngularVelocity* msgx = (SMessageSetAngularVelocity*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setAngularVelocity(msgx->AngularVelocity);

				if (p_entity->physicsID > -1)
					m_Physics.SetAngularVelocity(p_entity->physicsID, msgx->AngularVelocity);

				delete p_msg;
				break;
			}
		case MSG_SETTORQUE:
			{
				SMessageSetTorque* msgx = (SMessageSetTorque*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->setTorque(msgx->Torque);

				if (p_entity->physicsID > -1)
					m_Physics.SetTorque(p_entity->physicsID, msgx->Torque);

				delete p_msg;
				break;
			}
		case MSG_TRANSLATE:
			{
				SMessageTranslate* msgx = (SMessageTranslate*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->translate(msgx->DeltaPosition);

				if (p_entity->physicsID > -1)
					m_Physics.SetPosition(p_entity->physicsID, p_entity->getPosition());

				delete p_msg;
				break;
			}
		case MSG_ACCELERATE:
			{
				SMessageAccelerate* msgx = (SMessageAccelerate*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->accelerate(msgx->DeltaVelocity);

				if (p_entity->physicsID > -1)
					m_Physics.SetLinearVelocity(p_entity->physicsID, p_entity->getVelocity());

				delete p_msg;
				break;
			}
		case MSG_IMPULSEFORCE:
			{
				SMessageImpulseForce* msgx = (SMessageImpulseForce*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->applyImpulseForce(msgx->DeltaMomentum);

				if (p_entity->physicsID > -1)
					m_Physics.SetLinearVelocity(p_entity->physicsID, p_entity->getVelocity());

				delete p_msg;
				break;
			}
		case MSG_ADDFORCE:
			{
				SMessageAddForce* msgx = (SMessageAddForce*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->applyForce(msgx->DeltaForce);

				if (p_entity->physicsID > -1)
					m_Physics.ApplyCentralForce(p_entity->physicsID, msgx->DeltaForce);

				delete p_msg;
				break;
			}
		case MSG_ROTATEONAXIS:
			{
				SMessageAddRotationOnAxis* msgx = (SMessageAddRotationOnAxis*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				D3DXQUATERNION dRot; D3DXQuaternionRotationAxis(&dRot, &(msgx->Axis), msgx->Degree);//////////////////////////////////////////////////////////////////////////////////////////
				p_entity->rotate(dRot);

				if (p_entity->physicsID > -1)
					m_Physics.SetRotation(p_entity->physicsID, p_entity->getRotation());

				delete p_msg;
				break;
			}
		case MSG_ROTATE_Q:
			{
				SMessageAddRotationQuat* msgx = (SMessageAddRotationQuat*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->rotate(msgx->Rotation);

				if (p_entity->physicsID > -1)
					m_Physics.SetRotation(p_entity->physicsID, p_entity->getRotation());

				delete p_msg;
				break;
			}
		case MSG_ROTATEONX:
			{
				SMessageAddRotationOnX* msgx = (SMessageAddRotationOnX*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if(p_entity->physicsID > -1)
					m_Physics.RotateOnCoordAxis(p_entity->physicsID, msgx->Degree, X);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}
		case MSG_ROTATEONY:
			{
				SMessageAddRotationOnY* msgx = (SMessageAddRotationOnY*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if (p_entity->physicsID > -1)
					m_Physics.RotateOnCoordAxis(p_entity->physicsID, msgx->Degree, Y);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}
		case MSG_ROTATEONZ:
			{
				SMessageAddRotationOnZ* msgx = (SMessageAddRotationOnZ*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if (p_entity->physicsID > -1)
					m_Physics.RotateOnCoordAxis(p_entity->physicsID, msgx->Degree, Z);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}

		case MSG_ADDANGULARVELOCITY:
			{
				SMessageAddAngularVelocity* msgx = (SMessageAddAngularVelocity*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				D3DXVECTOR3 axis; D3DXVec3Normalize(&axis, &(msgx->Axis));

				p_entity->applyAngularVelocity(axis * msgx->DeltaDegree);

				if (p_entity->physicsID > -1)
					m_Physics.SetAngularVelocity(p_entity->physicsID, p_entity->getAngularVelocity());

				delete p_msg;
				break;
			}
		case MSG_IMPULSETORQUE:
			{
				SMessageAddImpulseTorque* msgx = (SMessageAddImpulseTorque*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				D3DXVECTOR3 axis; D3DXVec3Normalize(&axis, &(msgx->Axis));

				p_entity->applyImpulseRotation(axis * msgx->AngularMomentum);

				if (p_entity->physicsID > -1)
					m_Physics.SetAngularVelocity(p_entity->physicsID, p_entity->getAngularVelocity());

				delete p_msg;
				break;
			}
		case MSG_ADDTORQUE:
			{
				SMessageAddTorque* msgx = (SMessageAddTorque*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				D3DXVECTOR3 axis; D3DXVec3Normalize(&axis, &(msgx->Axis));
				D3DXVECTOR3 torque = axis * msgx->DeltaTorque;

				p_entity->applyTorque(torque);

				if (p_entity->physicsID > -1)
					m_Physics.ApplyTorque(p_entity->physicsID, torque);

				delete p_msg;
				break;
			}
		case MSG_INCSCRIPTFLAG1:
			{
				SMessageIncScriptFlag1* msgx = (SMessageIncScriptFlag1*)p_msg;
				int eID = msgx->ID;
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				++(p_entity->scriptFlag1);

				delete p_msg;
				break;
			}

			/*
			MSG_INCSCRIPTFLAG2,
			*/

		case MSG_SETSTEERING:
			{
				SMessageSetSteering* msgx = (SMessageSetSteering*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->steeringType = msgx->SteeringType;

				delete p_msg;
				break;
			}
		default:
			{
				tempQueue.push(p_msg);
				break;
			}
		}
		m_QueueBuffer.pop();
	}
	m_QueueBuffer = tempQueue;
}

void EntityCore::PhysicsMsg()
{
}

void EntityCore::Export()
{
	EntityList entityList;
	m_EntityMgr.CloneInto(entityList);

	//GraphicsCore export
	vector<DD_StaticMeshData> staticMeshExport;
	//ScriptingCore export
	vector<ScriptData> scriptExport;
	//Picking export
	DDPickingData Pickingtemp;
	//Position export
	vector<DDGameData> posExport;
	//Steering export
	vector<DDSteeringData> steerExport;
	vector<SteeringObstacles> obstacleExport;

	EntityList::iterator end = entityList.end();
	for (EntityList::iterator itr = entityList.begin(); itr != end; ++itr)
	{
		EntityData entityData = *itr;
		//Export to Graphics
		switch(gEPMgr->GetItem(entityData.type)->RenderType)
		{
		case SCENE_STATICMESH:
			{
				//Export Graphics
				DD_StaticMeshData SMDtemp;
				TranslateToStaticMesh(entityData, SMDtemp);
				staticMeshExport.push_back(SMDtemp);
				//Export scripting
				ScriptData SDtemp;
				TranslateToScriptData(entityData, SDtemp);
				scriptExport.push_back(SDtemp);
				//Export picking
				Pickingtemp.ID.push_back(entityData.ID);
				Pickingtemp.Position.push_back(entityData.position);
				Pickingtemp.Halfscale.push_back(gEPMgr->GetItem(entityData.type)->mat->scalar);
				break;
			}
		}
		//Export to Physics
		if(entityData.physicsID > -1)
		{
			m_Physics.ApplyCentralForce(entityData.physicsID, entityData.force);
		}
		//if(entityData.Type == EP_SMALLMARBLE || entityData.Type == EP_MEDIUMMARBLE ||
		//	entityData.Type == EP_LARGEMARBLE || entityData.Type == EP_PLAYERMARBLE)
		{
			DDGameData temp;
			temp.ID = entityData.ID;
			temp.Type = entityData.type;
			temp.Position = entityData.position;
			posExport.push_back(temp);
		}

		//Export to AI
		if(entityData.steeringType &&
			(entityData.type == EP_SMALLMARBLE || entityData.type == EP_MEDIUMMARBLE ||
			entityData.type == EP_LARGEMARBLE))
		{
			DDSteeringData temp;
			temp.ID = entityData.ID;
			temp.m_dSpeed = D3DXVec3Length(&(entityData.velocity));
			temp.m_iEvadeID = 0;
			temp.m_iPursuitID = 0;
			temp.SHeading = SVector3D(0, 0);
			temp.SPosition = SVector3D(entityData.position.x, entityData.position.z);
			temp.SteeringType = entityData.steeringType;
			temp.SVelocity = SVector3D(entityData.velocity.x, entityData.velocity.z);

			steerExport.push_back(temp);
		}
		if(entityData.type == EP_WALL || entityData.type == EP_BOX)
		{
			SteeringObstacles temp;
			temp.m_iID = entityData.ID;
			temp.m_dRadius = gEPMgr->GetItem(entityData.type)->mat->scalar.x;
			temp.m_dScale = 1;
			temp.m_dTagged = false;
			temp.SPosition = SVector3D(entityData.position.x, entityData.position.z);
			obstacleExport.push_back(temp);
		}
	}
	m_PositionExport.Overwrite(posExport);
	m_StaticMeshExport.Overwrite(staticMeshExport);
	m_ScriptExport.Overwrite(scriptExport);
	m_PickingExport.Overwrite(Pickingtemp);
	m_SteeringExport.Overwrite(steerExport);
	m_ObstacleExport.Overwrite(obstacleExport);
}

void EntityCore::PollPhysics()
{
	EntityList::iterator end = m_EntityMgr.End();
	for (EntityList::iterator itr = m_EntityMgr.Begin(); itr != end; ++itr)
	{
		EntityData& entityData = *(itr);

		if (entityData.physicsID > -1)
		{
			entityData.position = m_Physics.GetPosition(entityData.physicsID);
			entityData.velocity = m_Physics.GetLinearVelocity(entityData.physicsID);
			entityData.rotation = m_Physics.GetRotation(entityData.physicsID);
		}
	}
}

/*
This function translates the entity into static mesh
takes in and entity + id and DD_StaticMeshData, both reference
outputs onto the second parameter
*/
void EntityCore::TranslateToStaticMesh(EntityData &entity, DD_StaticMeshData &out)
{
	out.MeshID = gEPMgr->GetItem(entity.type)->GResourceID;
	out.TextureID = 0;
	D3DXMATRIX m;
	D3DXMatrixTransformation(&m, &D3DXVECTOR3(0,0,0), 0, 
		&(gEPMgr->GetItem(entity.type)->GraphicsScale), &D3DXVECTOR3(0,0,0), 
		&(entity.rotation), &(entity.position));
	out.Transform = m;
}

/*
This function translates the entity into scripting data
takes in an entity + id and ScriptData, both reference as inputs
outputs onto the second parameter
*/
void EntityCore::TranslateToScriptData(EntityData &entity, ScriptData &out)
{
	out.objID = entity.ID;
	out.objType = gEPMgr->GetItem(entity.type)->ScriptName;
	out.LMouseClick = entity.scriptFlag1;
	out.RMouseClick = entity.scriptFlag2;
	out.Collision = 0;
}
