#pragma once

#include "Prerequisites.hpp"

#include "Module_Debug.hpp"
#include "../Components/Collider.hpp"
#include "../Components/RigidBody.hpp"

namespace SpaceMarines
{

class Collider;
class RigidBody;

namespace RayCastMode
{ enum List {
	booleanOnly = 0,
	GetCollisionInfo = 1
};}

struct RayCastHit
{
	bool hasHit;
	Collider* collider;
	Vector3 point;
	Vector3 normal;
};

/*************************************
 * 	Base PhysicsWorld class
 *************************************/
namespace Module
{

class PhysicsWorld
{
public:
	virtual ~PhysicsWorld();
	btDiscreteDynamicsWorld* getBulletWorld();

	void removeRigidBody(RigidBody* rigidBody);

	void fixedUpdate();
	void setDebugDrawer(Module::DebugDrawer* debugDrawer);
	void debugDraw();

	bool rayCast(const Vector3 &start, const Vector3 &direction, const float distance) const;
	bool rayCast(const Vector3 &start, const Vector3 &end) const;
	bool rayCast(const Ray &ray, const float distance = 100.0f) { return rayCast(ray.origin, ray.direction, distance); }
	RayCastHit rayCastComplex(const Vector3 &start, const Vector3 &direction, const float distance) const;
	RayCastHit rayCastComplex(const Vector3 &start, const Vector3 &end) const;
	RayCastHit rayCastComplex(const Ray &ray, const float distance = 100.0f) { return rayCastComplex(ray.origin, ray.direction, distance); }

	bool sphereCast(const Vector3 &start, const Vector3&direction, const float distance, const float radius) const;

	PhysicsWorld();
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* defaultCollisionConfiguration;
	btCollisionDispatcher* collisionDispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};




}
} /* namespace SpaceMarines */
