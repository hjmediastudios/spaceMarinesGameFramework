#pragma once
#include "Prerequisites.hpp"
#include "Renderer.hpp"

namespace SpaceMarines
{

class Renderer;

class DebugDrawer
{
public:
	Shader* debugShader;
	DebugDrawer(Renderer* renderer);
	~DebugDrawer();
	void drawLine(const Vector3 &ptA, const Vector3 &ptB, const Vector3 &color = Vector3::ONE);
//	void drawAxis(const Vector3 &point, const float size = 1.0f, const Quaternion &rotation = Quaternion::IDENTITY, const Vector3 &color = Vector3::ONE);
protected:
	void render();
	friend class Renderer;
private:
	Renderer* renderer;
	GLuint LlinesVBO;
	GLuint LlinesIBO;
	GLuint Ldb_mvpMat;

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

	std::vector<DebugVertex> vertices;
	std::vector<unsigned int> indices;

};

}
