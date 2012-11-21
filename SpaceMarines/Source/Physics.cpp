/*
 * Physics.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: nick
 */

#include "Physics.hpp"

namespace SpaceMarines
{

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

} /* namespace SpaceMarines */
