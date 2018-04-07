#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"
#include "Util.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "PhysicsMat.h"
//#include <stdio.h>

//Heavily Edited done by - TP   (to fix poorly organized methods, bad use of conversions, and adding new features)

enum AxisID
{
	X,
	Y,
	Z
};

class PhysicsWorld
{
private:
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* p_collisionConfiguration;

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* p_dispatcher;

	btAxisSweep3* p_overlappingPairCache;

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* p_solver;

	btDiscreteDynamicsWorld* p_dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> p_collisionShapes;

	int numOfObjects;

public:
	PhysicsWorld();
	~PhysicsWorld();

	void CreateWorld(WorldInfo& WldInfo);
	void ClearPhysics();

	void UpdateWorld(btScalar timeStep = (1 / 60), int maxSubSteps = 1);

	void DisplayCallback();

	int CreatePhysics_Object(PhysicsMat& pMat, D3DXVECTOR3 position);

	void DeletePhysicsObject(int id);

	bool IsKinematic(int id);
	void SetIsKinematic(int id, TypePhysics type);

	void setPhysicsMat(int id, const PhysicsMat& mat);

	void SetConstraintsLinear(int id, bool x, bool y, bool z);
	void SetConstraintsAngular(int id, bool x, bool y, bool z);

	D3DXVECTOR3 GetPosition(int id);
	void SetPosition(int id, D3DXVECTOR3& position);

	D3DXVECTOR3 GetLinearVelocity(int id);
	void SetLinearVelocity(int id, D3DXVECTOR3& vel);

	void SetCentralForce(int id, D3DXVECTOR3& force);
	void ApplyCentralForce(int id, D3DXVECTOR3& force);
	
	D3DXQUATERNION GetRotation(int id);
	void SetRotation(int id, D3DXQUATERNION& quat);
	void SetRotation(int id, D3DXVECTOR3& axis,float degree);
	void RotateOnCoordAxis(int id, float angle, AxisID axis);

	D3DXVECTOR3 GetAngularVelocity(int id);
	void SetAngularVelocity(int id, D3DXVECTOR3& aVel);

	void SetTorque(int id, D3DXVECTOR3& torque);
	void ApplyTorque(int id, D3DXVECTOR3& torque);

	btVector3 ConvertToBtVec(const D3DXVECTOR3& old);
	D3DXVECTOR3 ConvertToDxVec(const btVector3& old);

	btQuaternion ConvertToBtQuat(const D3DXQUATERNION& old);
	D3DXQUATERNION ConvertToDxRot(const btQuaternion& old);

	void SetPhysics();

private:
	btCollisionShape* createCollisionShape(const PhysicsMat& pMat);

	btQuaternion GetBtRotation(int id);
	void SetBtRotation(int id, btQuaternion& quat);
};

extern PhysicsWorld* gp_physics;

#endif
