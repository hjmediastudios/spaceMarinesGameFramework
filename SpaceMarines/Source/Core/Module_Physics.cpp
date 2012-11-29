/*
 * Physics.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: nick
 */

#include "Module_Physics.hpp"

namespace SpaceMarines
{


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
	std::cout << "Setting debug drawer!\n";
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



} /* namespace SpaceMarines */
