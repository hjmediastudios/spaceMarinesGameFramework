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
	void start()
	{
		collisionShape = createCollisionShape();
		collisionShape->setUserPointer(this);
	}
	const char* getComponentType() const { return "Collider"; }
protected:
	btCollisionShape* collisionShape;
	virtual btCollisionShape* createCollisionShape() = 0;
	friend class RigidBody;
	Vector3 offset;
};

/*************************************
 * 	Collider classes
 *************************************/
class BoxCollider : public Collider
{
public:
	BoxCollider(const Vector3 &extents = Vector3::ONE, const Vector3& offset = Vector3::ZERO) : Collider(offset) { this->extents = extents;}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btBoxShape(extents.bullet() * 0.5f);
	}
private:
	Vector3 extents;
};

class GroundPlaneCollider : public Collider
{
public:
	GroundPlaneCollider(const Vector3& offset = Vector3::ZERO) : Collider(offset) {}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btStaticPlaneShape(Vector3::UP.bullet(), offset.z);
	}
};

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(const float radius, const float height, const Vector3 &offset = Vector3::ZERO) : Collider(offset)
	{
		params = Vector2(radius, height * 0.5f);
	}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btCapsuleShape(params.x, params.y);
	}
private:
	Vector2 params;
};


}
