#pragma once
#include "Prerequisites.hpp"
#include "Components/Collider.hpp"
#include "DebugDrawer.hpp"

namespace SpaceMarines
{

class RigidBody;

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
class RigidBody : public PassiveComponent, public HasFixedUpdate
{
public:
	RigidBody(Collider* collider, float mass = 1.0f, bool noSleep = false);
	~RigidBody();
	const char* getComponentType() const { return "RigidBody"; }
	void start();
	void fixedUpdate();
	Vector3 getVelocity() const;

	void applyForce(const Vector3 &force);
	void applyForceAtRelativePoint(const Vector3 &force, const Vector3 &point);

private:
	PhysicsWorld* world;
	Collider* collider;
	float mass;
	btRigidBody* rigidBody;
	friend class PhysicsWorld;
	Transform* transform;
	bool noSleep;
};



} /* namespace SpaceMarines */
