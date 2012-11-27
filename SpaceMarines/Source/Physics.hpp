#pragma once
#include "Prerequisites.hpp"
#include "Components/Collider.hpp"
#include "DebugDrawer.hpp"

namespace SpaceMarines
{
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
class PhysicsWorld
{
public:
	static PhysicsWorld* getSingleton();
	virtual ~PhysicsWorld();
	btDiscreteDynamicsWorld* getBulletWorld();

	void removeRigidBody(RigidBody* rigidBody);

	void fixedUpdate();
	void setDebugDrawer(DebugDrawer* debugDrawer);
	void debugDraw();

	bool rayCast(const Vector3 &start, const Vector3 &direction, const float distance) const;
	bool rayCast(const Vector3 &start, const Vector3 &end) const;
	bool rayCast(const Ray &ray, const float distance = 100.0f) { return rayCast(ray.origin, ray.direction, distance); }
	RayCastHit rayCastComplex(const Vector3 &start, const Vector3 &direction, const float distance) const;
	RayCastHit rayCastComplex(const Vector3 &start, const Vector3 &end) const;
	RayCastHit rayCastComplex(const Ray &ray, const float distance = 100.0f) { return rayCastComplex(ray.origin, ray.direction, distance); }

	bool sphereCast(const Vector3 &start, const Vector3&direction, const float distance, const float radius) const;

private:
	static PhysicsWorld* instance;
	PhysicsWorld();

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* defaultCollisionConfiguration;
	btCollisionDispatcher* collisionDispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

/*************************************
 * 	Rigid body component
 *************************************/
namespace ForceMode
{ enum List {
	Force = 0,
	Impulse = 1,
	Velocity = 2,
	Acceleration = 3
};}
class RigidBody : public PassiveComponent, public HasFixedUpdate
{
public:
	RigidBody(Collider* collider, float mass = 1.0f, bool noSleep = false);
	~RigidBody();
	const char* getComponentType() const { return "RigidBody"; }
	void start();
	void fixedUpdate();
	Vector3 getVelocity() const;

	void applyForce(const Vector3 &force, ForceMode::List mode = ForceMode::Force);
	void applyForceAtRelativePoint(const Vector3 &force, const Vector3 &point, ForceMode::List mode = ForceMode::Force);

	void setKeepUpright(bool lock);
private:
	PhysicsWorld* world;
	Collider* collider;
	float mass;
	btRigidBody* rigidBody;
	friend class PhysicsWorld;
	Transform* transform;
	bool noSleep;
	bool keepUpright;
};



} /* namespace SpaceMarines */
