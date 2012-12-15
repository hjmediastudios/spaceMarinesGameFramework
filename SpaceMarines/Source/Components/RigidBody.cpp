#include "RigidBody.hpp"
#include "../Core/Core.hpp"

namespace SpaceMarines
{
/*************************************
 * 	Rigid body component
 *************************************/
RigidBody::RigidBody(Collider* collider, float mass, bool noSleep)
{
	this->collider = collider;
	this->mass = mass;

	transform = nullptr;
	rigidBody = nullptr;
	this->noSleep = noSleep;
	lockRotation = false;
}

RigidBody::~RigidBody()
{
	Modules::physics().getBulletWorld()->removeRigidBody(this->rigidBody);
	if (rigidBody == nullptr) return;

	delete rigidBody->getMotionState();
	delete rigidBody;
}

void RigidBody::registerStart()
{
	if (gameObject == nullptr) throw Exception("RigidBody doesn't have a GameObject");
	if (collider == nullptr) throw Exception("RigidBody has no associated collider");
	if (collider->collisionShape == nullptr) throw Exception("Collider's Bullet collider is null");

	gameObject->eventFixedUpdate.addListener<RigidBody, &RigidBody::fixedUpdate>(this);

	transform = gameObject->getTransform();
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(
			gameObject->getTransform()->rotation.bullet(),
			gameObject->getTransform()->position.bullet()
			));

	btVector3 inertia;
	collider->collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, groundMotionState, collider->collisionShape, inertia);
	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setUserPointer(this);
	Modules::physics().getBulletWorld()->addRigidBody(rigidBody);
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

void RigidBody::setVelocity(const Vector3 &vel)
{
	rigidBody->setLinearVelocity(vel.bullet());
}

Vector3 RigidBody::getVelocityHorizontal() const
{
	btVector3 vel = rigidBody->getLinearVelocity();
	vel.setY(0);
	return vel;
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

}
