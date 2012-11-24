/*
 * Physics.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: nick
 */

#include "Physics.hpp"

namespace SpaceMarines
{

PhysicsWorld* PhysicsWorld::instance = nullptr;

PhysicsWorld* PhysicsWorld::getSingleton()
{
	if (instance == nullptr) instance = new PhysicsWorld();
	return instance;
}

PhysicsWorld::PhysicsWorld()
{
	broadphase = new btDbvtBroadphase();
	defaultCollisionConfiguration = new btDefaultCollisionConfiguration();
	collisionDispatcher = new btCollisionDispatcher(defaultCollisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver, defaultCollisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
}

PhysicsWorld::~PhysicsWorld()
{
	delete dynamicsWorld;
	delete solver;
	delete collisionDispatcher;
	delete defaultCollisionConfiguration;
	delete broadphase;
}

btDiscreteDynamicsWorld* PhysicsWorld::getBulletWorld()
{
	return dynamicsWorld;
}

void PhysicsWorld::removeRigidBody(RigidBody* rigidBody)
{
	if (rigidBody == nullptr) throw Exception("Attempting to remove a null RigidBody to PhysicsWorld");
	dynamicsWorld->removeRigidBody(rigidBody->rigidBody);
}

void PhysicsWorld::fixedUpdate()
{
	dynamicsWorld->stepSimulation(Time::fixedDeltaTimeF, 10);
}

/*************************************
 * 	Rigid body component
 *************************************/
RigidBody::RigidBody(Collider* collider, float mass)
{
	this->collider = collider;
	this->mass = mass;

	transform = nullptr;
	rigidBody = nullptr;
}

RigidBody::~RigidBody()
{
	if (world != nullptr)
		world->removeRigidBody(this);
	if (rigidBody == nullptr) return;

	delete rigidBody->getMotionState();
	delete rigidBody;
}

void RigidBody::start()
{
	if (gameObject == nullptr) throw Exception("RigidBody doesn't have a GameObject");
	if (collider == nullptr) throw Exception("RigidBody has no associated collider");
	if (collider->collisionShape == nullptr) throw Exception("Collider's Bullet collider is null");

	transform = gameObject->getTransform();
	world = PhysicsWorld::getSingleton();

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(
			gameObject->getTransform()->rotation.bullet(),
			gameObject->getTransform()->position.bullet()
			));

	btVector3 inertia;
	collider->collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, groundMotionState, collider->collisionShape, inertia);
	rigidBody = new btRigidBody(rigidBodyCI);

	PhysicsWorld::getSingleton()->getBulletWorld()->addRigidBody(rigidBody);
}

void RigidBody::fixedUpdate()
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);

	transform->setRotation(trans.getRotation());
	transform->setPosition(Vector3(trans.getOrigin()) + transform->rotation * -collider->offset);
}

Vector3 RigidBody::getVelocity() const
{
	return rigidBody->getLinearVelocity();
}

} /* namespace SpaceMarines */
