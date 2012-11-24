#pragma once
#include "Prerequisites.hpp"
#include "Components/Collider.hpp"

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
	RigidBody(Collider* collider, float mass = 1.0f);
	~RigidBody();
	const char* getComponentType() const { return "RigidBody"; }
	void start();
	void fixedUpdate();
	Vector3 getVelocity() const;
private:
	PhysicsWorld* world;
	Collider* collider;
	float mass;
	btRigidBody* rigidBody;
	friend class PhysicsWorld;
	Transform* transform;
};



} /* namespace SpaceMarines */
