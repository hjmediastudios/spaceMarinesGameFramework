#pragma once
#include "../Physics.hpp"

namespace SpaceMarines
{

/*************************************
 * 	Base Collider class
 *************************************/
class Collider : public PassiveComponent
{
public:
	Collider() { collisionShape = nullptr; }
	~Collider() { if (collisionShape != nullptr) delete collisionShape; }
	virtual void start() {}
	const char* getComponentType() const { return "Collider"; }
protected:
	btCollisionShape* collisionShape;
	virtual void createCollisionShape() = 0;
	friend class RigidBody;
};

/*************************************
 * 	BoxCollider class
 *************************************/
class BoxCollider : public Collider
{
public:
	BoxCollider() : Collider() {}
protected:
	void createCollisionShape()
	{
		collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	}
};


}
