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
	dynamicsWorld->stepSimulation(Time::fixedDeltaTime, 10);
}

void PhysicsWorld::setDebugDrawer(DebugDrawer* debugDrawer)
{
	dynamicsWorld->setDebugDrawer(debugDrawer);
	debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void PhysicsWorld::debugDraw()
{
	if (dynamicsWorld->getDebugDrawer() == nullptr) return;
	dynamicsWorld->debugDrawWorld();
}

bool PhysicsWorld::rayCast(const Vector3 &start, const Vector3&direction, const float distance) const
{
	return rayCast(start, start + direction * distance);
}

bool PhysicsWorld::rayCast(const Vector3 &start, const Vector3 &end) const
{
	btDynamicsWorld::ClosestRayResultCallback callback(start.bullet(), end.bullet());
	dynamicsWorld->rayTest(start.bullet(), end.bullet(), callback);
	return callback.hasHit();
}

RayCastHit PhysicsWorld::rayCastComplex(const Vector3 &start, const Vector3 &end) const
{
	RayCastHit hit;
	btDynamicsWorld::ClosestRayResultCallback callback(start.bullet(), end.bullet());
	dynamicsWorld->rayTest(start.bullet(), end.bullet(), callback);
	if (callback.hasHit())
	{
		hit.hasHit = true;
		hit.collider = (Collider*) callback.m_collisionObject->getUserPointer();
		hit.point = callback.m_hitPointWorld;
		hit.normal = callback.m_hitNormalWorld;
	}
	else
		hit.hasHit = false;

	return hit;
}

RayCastHit PhysicsWorld::rayCastComplex(const Vector3 &start, const Vector3 &direction, float distance) const
{
	return rayCastComplex(start, start + direction * distance);
}

bool PhysicsWorld::sphereCast(const Vector3 &start, const Vector3 &direction, float distance, float radius) const
{
//	dynamicsWorld->convexSweepTest() ToDo SphereCast
}

/*************************************
 * 	Rigid body component
 *************************************/
RigidBody::RigidBody(Collider* collider, float mass, bool noSleep)
{
	this->collider = collider;
	this->mass = mass;

	transform = nullptr;
	rigidBody = nullptr;
	world = nullptr;
	this->noSleep = noSleep;
	lockRotation = false;
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
	rigidBody->setUserPointer(this);
	PhysicsWorld::getSingleton()->getBulletWorld()->addRigidBody(rigidBody);
	setLockRotation(lockRotation);
}

void RigidBody::fixedUpdate()
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);

	if (!lockRotation) transform->setRotation(trans.getRotation());
	transform->setPosition(Vector3(trans.getOrigin()) + transform->rotation * -collider->offset);
}

Vector3 RigidBody::getVelocity() const
{
	return rigidBody->getLinearVelocity();
}

void RigidBody::applyForce(const Vector3 &force, ForceMode::List mode)
{
	if (!rigidBody->isActive()) rigidBody->activate();
	switch (mode)
	{
	case ForceMode::Force:
		rigidBody->applyCentralForce(force.bullet());
		break;
	case ForceMode::Velocity:
		rigidBody->setLinearVelocity(force.bullet());
		break;
	case ForceMode::Acceleration:
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + force.bullet());
		break;
	case ForceMode::Impulse:
		rigidBody->applyCentralImpulse(force.bullet());
		break;
	default:
		rigidBody->applyCentralForce(force.bullet());
		break;
	}
}

void RigidBody::applyForceAtRelativePoint(const Vector3 &force, const Vector3 &point, ForceMode::List mode)
{
	if (!rigidBody->isActive()) rigidBody->activate();
	switch (mode)
	{
	case ForceMode::Force:
		rigidBody->applyForce(force.bullet(), point.bullet());
		break;
	case ForceMode::Velocity:
		rigidBody->setLinearVelocity(force.bullet());
		break;
	case ForceMode::Impulse:
		rigidBody->applyImpulse(force.bullet(), point.bullet());
		break;
	case ForceMode::Acceleration:
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + force.bullet());
		break;
	default:
		rigidBody->applyCentralForce(force.bullet());
		break;
	}
}

void RigidBody::setLockRotation(bool lock)
{
	lockRotation = lock;
	if (rigidBody == nullptr) return;
	else
	{
		rigidBody->setAngularFactor(btVector3(0, 1, 0));
	}
}

} /* namespace SpaceMarines */
