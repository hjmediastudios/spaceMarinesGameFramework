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

void PhysicsWorld::addRigidBody(RigidBody* rigidBody)
{
	dynamicsWorld->addRigidBody(rigidBody->rigidBody);
}

/*************************************
 * 	Rigid body component
 *************************************/
RigidBody::RigidBody(Collider* collider, float mass)
{
	this->collider = collider;
	this->mass = mass;

	rigidBody = nullptr;
}

RigidBody::~RigidBody()
{
	if (rigidBody != nullptr) delete rigidBody;
}

void RigidBody::start()
{
	if (gameObject == nullptr) throw Exception("RigidBody doesn't have a GameObject");
	if (collider == nullptr) throw Exception("RigidBody has no associated collider");


	btVector3 fallInteria = btVector3(0, 0, 0);
	btDefaultMotionState defaultMotionState = btDefaultMotionState(btTransform(
			gameObject->getTransform()->getRotation().bullet(),
			gameObject->getTransform()->getPosition().bullet()));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, &defaultMotionState, collider->collisionShape, fallInteria);
	rigidBody = new btRigidBody(constructionInfo);

	PhysicsWorld::getSingleton()->addRigidBody(this);
	std::cout << "Starting RigidBody\n";
}

} /* namespace SpaceMarines */
