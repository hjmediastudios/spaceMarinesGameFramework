#pragma once

#include "Prerequisites.hpp"

namespace SpaceMarines
{

struct DebugVertex
{
	Vector3 pos;
	Vector3 color;
	DebugVertex() {}
	DebugVertex(const Vector3 &pos, const Vector3 &color)
	{
		this->pos = pos;
		this->color = color;
	}
};

class Rectangle
{
protected:
	Vector2 position;
	Vector2 extents;
public:
	Rectangle(const Vector2& position, const Vector2& extents)
	{
		this->position = position;
		this->extents = extents;
	}
};

}
