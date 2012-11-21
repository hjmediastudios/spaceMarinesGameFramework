#pragma once
#include "Prerequisites.hpp"
#include <bullet/btBulletDynamicsCommon.h>

namespace SpaceMarines
{

class PhysicsWorld
{
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* defaultCollisionConfiguration;
	btCollisionDispatcher* collisionDispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

} /* namespace SpaceMarines */
