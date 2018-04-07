#include "Physics.h"

//Heavily Edited done by - TP  (to fix poorly organized methods, bad use of conversions, and adding new features)

PhysicsWorld* gp_physics = 0;


PhysicsWorld::PhysicsWorld(){
	numOfObjects=-1;
}

PhysicsWorld::~PhysicsWorld()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	//for (int i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	for (int i = p_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* p_body = btRigidBody::upcast(p_obj);
		if (p_body && p_body->getMotionState())
		{
			delete p_body->getMotionState();
		}
		p_dynamicsWorld->removeCollisionObject(p_obj);
		delete p_obj;
	}

	//delete collision shapes
	for (int j = 0; j<p_collisionShapes.size(); j++)
	{
		btCollisionShape* p_shape = p_collisionShapes[j];
		p_collisionShapes[j] = 0;
		delete p_shape;
	}

	//delete dynamics world
	delete p_dynamicsWorld;

	//delete solver
	delete p_solver;

	//delete broadphase
	delete p_overlappingPairCache;

	//delete dispatcher
	delete p_dispatcher;

	delete p_collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	p_collisionShapes.clear();

}




void PhysicsWorld::CreateWorld(WorldInfo& WldInfo)
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	p_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	p_dispatcher = new	btCollisionDispatcher(p_collisionConfiguration);

	///the maximum size of the collision world. Make sure objects stay within these boundaries
	///Don't make the world AABB size too large, it will harm simulation quality and performance
	btVector3 worldAabbMin=WldInfo.WorldAabbMin;
	btVector3 worldAabbMax=WldInfo.WorldAabbMax;
	int	maxProxies = 1024;
	
	p_overlappingPairCache = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	p_solver = new btSequentialImpulseConstraintSolver;

	p_dynamicsWorld = new btDiscreteDynamicsWorld(p_dispatcher,p_overlappingPairCache,p_solver,p_collisionConfiguration);

	p_dynamicsWorld->setGravity(WldInfo.Gravity);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
}

void PhysicsWorld::ClearPhysics()
{
	for (int i = 7; i <= p_dynamicsWorld->getNumCollisionObjects() - 1; i++)
	{
		btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(p_obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		p_dynamicsWorld->removeCollisionObject(p_obj);
		delete p_obj;
	}


}




void PhysicsWorld::UpdateWorld(btScalar timeStep, int maxSubSteps)
{
	p_dynamicsWorld->stepSimulation(timeStep, maxSubSteps);
}




void PhysicsWorld::DisplayCallback()
{
	//if (dynamicsWorld)
	//	dynamicsWorld->performDiscreteCollisionDetection();

	int i;

	///one way to draw all the contact points is iterating over contact manifolds / points:

	int numManifolds = p_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* p_contactManifold = p_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* p_obA = static_cast<btCollisionObject*>(p_contactManifold->getBody0());
		btCollisionObject* p_obB = static_cast<btCollisionObject*>(p_contactManifold->getBody1());

		int numContacts = p_contactManifold->getNumContacts();
		for (int j = 0; j<numContacts; j++)
		{
			btManifoldPoint& pt = p_contactManifold->getContactPoint(j);

		}
	}
}



int PhysicsWorld::CreatePhysics_Object(PhysicsMat& pMat, D3DXVECTOR3 position)
{
	btCollisionShape* p_colShape = createCollisionShape(pMat);
	if(p_colShape == nullptr)
		return numOfObjects;
	
	p_collisionShapes.push_back(p_colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass = pMat.mass;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 inertiaTensor = ConvertToBtVec(pMat.inertiaTensor);
	if (isDynamic)
		p_colShape->calculateLocalInertia(mass, inertiaTensor);

	startTransform.setOrigin(ConvertToBtVec(position));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, p_colShape, inertiaTensor);
	
	rbInfo.m_friction = pMat.friction;
	rbInfo.m_restitution = pMat.restitution;
	rbInfo.m_linearDamping = pMat.linearDamping;
	rbInfo.m_angularDamping = pMat.angularDamping;
	btRigidBody* p_body = new btRigidBody(rbInfo);

	p_dynamicsWorld->addRigidBody(p_body);

	numOfObjects++;
	return numOfObjects;
}




void PhysicsWorld::DeletePhysicsObject(int id)
{
	/// works like a deque...remove one and they push forward...i think
	/// TP: NOPE it takes the back most remaining entry and moves it to the vacated position!!!!!!!
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	p_dynamicsWorld->removeCollisionObject(p_obj);
	--numOfObjects;
}




bool PhysicsWorld::IsKinematic(int id)
{
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);

	return p_body->isKinematicObject();
}

void PhysicsWorld::SetIsKinematic(int id, TypePhysics type)
{
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		switch (type)
		{
		case PHYSICS_STATIC:
			p_body->setCollisionFlags((p_body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT) & ~btCollisionObject::CF_KINEMATIC_OBJECT);
			break;
		case PHYSICS_DYNAMIC:
			p_body->setCollisionFlags(p_body->getCollisionFlags() & ~(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT));
			break;
		case PHYSICS_KINEMATIC:
			p_body->setCollisionFlags((p_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT) & ~btCollisionObject::CF_STATIC_OBJECT);
			break;
		default:
			return;
		}
		p_body->setActivationState(DISABLE_DEACTIVATION);
	}
}




void PhysicsWorld::setPhysicsMat(int id, const PhysicsMat& pMat)
{
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);

	if (p_body && p_body->getMotionState())
	{
		btVector3 inertiaTensor = ConvertToBtVec(pMat.inertiaTensor);

		btCollisionShape* p_colShape = createCollisionShape(pMat);
		if (p_colShape != nullptr)
		{
			btCollisionShape* p_oldShape = p_body->getCollisionShape();
			int size = p_collisionShapes.size();
			for (int i = 0; i <= size; ++i)
			{
				if (p_collisionShapes[i] == p_oldShape)
				{
					p_collisionShapes[i] = p_colShape;
					delete p_oldShape;
					break;
				}
			}

			bool isDynamic = (pMat.mass != 0.f);
			if (isDynamic)
				p_colShape->calculateLocalInertia(pMat.mass, inertiaTensor);

			p_body->setCollisionShape(p_colShape);
		}

		p_body->setMassProps(pMat.mass, inertiaTensor);
		p_body->setFriction(pMat.friction);
		p_body->setRestitution(pMat.restitution);
		p_body->setDamping(pMat.linearDamping, pMat.angularDamping);

		p_body->setActivationState(DISABLE_DEACTIVATION);
	}
}




void PhysicsWorld::SetConstraintsLinear(int id, bool x, bool y, bool z)
{
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);

	if (p_body && p_body->getMotionState())
	{
		p_body;// TODO: 
	}
}

void PhysicsWorld::SetConstraintsAngular(int id, bool x, bool y, bool z)
{
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);

	if (p_body && p_body->getMotionState())
	{
		p_body;// TODO: 
	}
}




btCollisionShape* PhysicsWorld::createCollisionShape(const PhysicsMat& pMat)
{
	btCollisionShape* p_colShape = nullptr;
	switch (pMat.type)
	{
	case COLLIDER_PLANE:
	{
		PlanePMat& plaPMat = (PlanePMat&)pMat;
		D3DXVECTOR4 normal = plaPMat.pNormal;

		p_colShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), normal.w);// TODO figure out how to include non-static version
		break;
	}
	case COLLIDER_BOX:
	{
		BoxPMat& boxPMat = (BoxPMat&)pMat;
		btVector3 size(ConvertToBtVec(boxPMat.scalar));

		p_colShape = new btBoxShape(size);
		break;
	}
	case COLLIDER_SPHERE:
	{
		SpherePMat& sphPMat = (SpherePMat&)pMat;

		p_colShape = new btSphereShape(sphPMat.radius);
		break;
	}
	case COLLIDER_CYLINDER:
	{
		CylinderPMat& cylPMat = (CylinderPMat&)pMat;
		btVector3 size(cylPMat.radius * cylPMat.scalar.x, cylPMat.length * cylPMat.scalar.y, cylPMat.radius * cylPMat.scalar.z);

		p_colShape = new btCylinderShape(size);
		break;
	}
	case COLLIDER_CAPSULE:
	{
		CapsulePMat& capPMat = (CapsulePMat&)pMat;

		p_colShape = new btCapsuleShape(capPMat.radius, capPMat.length);
		break;
	}
	}
	return p_colShape;
}




D3DXVECTOR3 PhysicsWorld::GetPosition(int id)
{
	D3DXVECTOR3 pos;

	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		btTransform trans;
		p_body->getMotionState()->getWorldTransform(trans);

		pos = ConvertToDxVec(trans.getOrigin());
	}
	return pos;

}

void PhysicsWorld::SetPosition(int id, D3DXVECTOR3& position)
{
	btVector3 btPosition = ConvertToBtVec(position);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		btTransform trans;
		p_body->getMotionState()->getWorldTransform(trans);
		trans.setOrigin(btPosition);
		p_body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		p_dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setOrigin(btPosition);
	}
}




D3DXVECTOR3 PhysicsWorld::GetLinearVelocity(int id)
{
	D3DXVECTOR3 vec;
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		vec = ConvertToDxVec(p_body->getLinearVelocity());
	}
	return vec;
}

void PhysicsWorld::SetLinearVelocity(int id, D3DXVECTOR3& vel)
{
	btVector3 btVel = ConvertToBtVec(vel);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		p_body->setLinearVelocity(btVel);
	}
}




void PhysicsWorld::SetCentralForce(int id, D3DXVECTOR3& force)
{
	btVector3 btForce = ConvertToBtVec(force);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		p_body->applyCentralForce(btForce - p_body->getTotalForce());
	}
}

void PhysicsWorld::ApplyCentralForce(int id, D3DXVECTOR3& force)
{
	btVector3 btForce = ConvertToBtVec(force);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		p_body->applyCentralForce(btForce);
	}
}




D3DXQUATERNION PhysicsWorld::GetRotation(int id)
{
	return ConvertToDxRot(GetBtRotation(id));
	/*
	D3DXQUATERNION rot;

	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		rot = convertToDxRot(trans.getRotation());
	}

	return rot;*/
}

btQuaternion PhysicsWorld::GetBtRotation(int id)
{
	btQuaternion rot;

	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		btTransform trans;
		p_body->getMotionState()->getWorldTransform(trans);

		rot = trans.getRotation();
	}

	return rot;
}

void PhysicsWorld::SetRotation(int id, D3DXQUATERNION& quat)
{
	SetBtRotation(id, ConvertToBtQuat(quat));
	/*
	btQuaternion btQuat = convertToBtQuat(quat);
	btCollisionObject* p_obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		btTransform trans;
		p_body->getMotionState()->getWorldTransform(trans);
		trans.setRotation(btQuat);
		p_body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setRotation(btQuat);
	}
	*/
}

void PhysicsWorld::SetBtRotation(int id, btQuaternion& quat)
{
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		btTransform trans;
		p_body->getMotionState()->getWorldTransform(trans);
		trans.setRotation(quat);
		p_body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		p_dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setRotation(quat);
	}
}

void PhysicsWorld::SetRotation(int id, D3DXVECTOR3& axis, float degree)
{
	degree = (3.14159265 / 180)*degree;
	btQuaternion rot(btVector3(axis.x, axis.y, axis.z), degree);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		btTransform trans;
		p_body->getMotionState()->getWorldTransform(trans);
		trans.setRotation(rot);
		p_body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		p_dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setRotation(rot);
	}
}

void PhysicsWorld::RotateOnCoordAxis(int id, float degree, AxisID axis)
{
	btQuaternion q1 = GetBtRotation(id);
	btVector3 vec;
	switch (axis)
	{
	case X:
		vec = btVector3(-20, 0, 0);
		break;
	case Y:
		vec = btVector3(0, -20, 0);
		break;
	case Z:
		vec = btVector3(0, 0, -20);
		break;
	}
	btQuaternion q2(vec, degree*(3.14159265 / 180));
	btQuaternion q3;
	q3 = q2*q1;

	SetBtRotation(id, q3);
}





D3DXVECTOR3 PhysicsWorld::GetAngularVelocity(int id)
{
	D3DXVECTOR3 vec;
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		vec = ConvertToDxVec(p_body->getAngularVelocity());
	}
	return vec;
}

void PhysicsWorld::SetAngularVelocity(int id, D3DXVECTOR3& aVel)
{
	btVector3 btAVel = ConvertToBtVec(aVel);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		p_body->setAngularVelocity(btAVel);
	}
}




void PhysicsWorld::SetTorque(int id, D3DXVECTOR3& torque)
{
	btVector3 btTorque = ConvertToBtVec(torque);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		p_body->applyTorque(btTorque - p_body->getTotalTorque());
	}
}

void PhysicsWorld::ApplyTorque(int id, D3DXVECTOR3& torque)
{
	btVector3 btTorque = ConvertToBtVec(torque);
	btCollisionObject* p_obj = p_dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* p_body = btRigidBody::upcast(p_obj);
	if (p_body && p_body->getMotionState())
	{
		p_body->applyTorque(btTorque);
	}
}




btVector3 PhysicsWorld::ConvertToBtVec(const D3DXVECTOR3& c_old)
{
	btVector3 newVec(c_old.x, c_old.y, c_old.z);

	return newVec;
}

D3DXVECTOR3 PhysicsWorld::ConvertToDxVec(const btVector3& c_old)
{
	D3DXVECTOR3 newVec(c_old.x(), c_old.y(), c_old.z());

	return newVec;
}




btQuaternion PhysicsWorld::ConvertToBtQuat(const D3DXQUATERNION& c_old)
{
	btQuaternion tempQuat(c_old.x, c_old.y, c_old.z, c_old.w);

	return tempQuat;
}

D3DXQUATERNION PhysicsWorld::ConvertToDxRot(const btQuaternion &c_old)
{
	D3DXQUATERNION tempQuat(c_old.x(), c_old.y(), c_old.z(), c_old.w());

	return tempQuat;
}




void PhysicsWorld::SetPhysics()
{
}
