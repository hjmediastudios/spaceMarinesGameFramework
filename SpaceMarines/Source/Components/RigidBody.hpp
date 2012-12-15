#pragma once

#include "../Core/Module_Physics.hpp"
#include "Collider.hpp"

namespace SpaceMarines
{

namespace ForceMode
{ enum List {
	Force = 0,
	Impulse = 1,
	Velocity = 2,
	Acceleration = 3
};}

class Collider;

namespace Module
{
	class PhysicsWorld;
}

class RigidBody : public Component
{
public:
	RigidBody(Collider* collider, float mass = 1.0f, bool noSleep = false);
	~RigidBody();
	const char* getComponentType() const { return "RigidBody"; }
	void registerStart();
	void fixedUpdate();
	Vector3 getVelocity() const;
	Vector3 getVelocityHorizontal() const;
	void setVelocity(const Vector3 &vel = Vector3::ZERO);

	void applyForce(const Vector3 &force, ForceMode::List mode = ForceMode::Force);
	void applyForceAtRelativePoint(const Vector3 &force, const Vector3 &point, ForceMode::List mode = ForceMode::Force);

	void setLockRotation(bool lock);
private:
	Collider* collider;
	float mass;
	btRigidBody* rigidBody;
	friend class Module::PhysicsWorld;
	Transform* transform;
	bool noSleep;
	bool lockRotation;
};

}
