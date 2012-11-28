#include "Object.hpp"
namespace SpaceMarines
{

void Transform::lookAt(const Vector3 &pt, const Vector3 &up)
{
	Vector3 dir = pt - position;

	if (dir.lengthSquared() > Math::Epsilon)
		lookInDirection(dir, up);
}

void Transform::lookInDirection(const Vector3 &direction, const Vector3 &up)
{
	//Don't attempt to point at a point that's right here
	if (direction.lengthSquared() <= (1.0f + Math::Epsilon)) return;
	//Don't point if the up vector and the point are colinear FixMe skew whendirection.lengthSquared <= 1.0f
	Vector3 dir = direction.normalized();
	Vector3 upVec = up.normalized();

	if (dir.dot(upVec) > 0.9f) return;

	setRotation(Quaternion::rotationToDirection(dir, upVec));
}


}
