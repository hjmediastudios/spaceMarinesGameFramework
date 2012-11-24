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
	Collider(const Vector3 &offset) { collisionShape = nullptr; this->offset = offset; }
	~Collider() { if (collisionShape != nullptr) delete collisionShape; }
	void start() {collisionShape = createCollisionShape();}
	const char* getComponentType() const { return "Collider"; }
protected:
	btCollisionShape* collisionShape;
	virtual btCollisionShape* createCollisionShape() = 0;
	friend class RigidBody;
	Vector3 offset;
};

/*************************************
 * 	BoxCollider class
 *************************************/
class BoxCollider : public Collider
{
public:
	BoxCollider(const Vector3& offset = Vector3::ZERO) : Collider(offset) {}
protected:
	btCollisionShape* createCollisionShape()
	{
		//ToDo custom limits
		return new btBoxShape(btVector3(0.5f, 1.0f, 0.1f));
	}
};

class StaticPlaneCollider : public Collider
{
public:
	StaticPlaneCollider(const Vector3& offset = Vector3::ZERO) : Collider(offset) {}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btStaticPlaneShape(Vector3::UP.bullet(), 1);
	}
};


}
