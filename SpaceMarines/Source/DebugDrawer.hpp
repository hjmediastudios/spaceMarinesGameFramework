#pragma once
#include "Prerequisites.hpp"
#include "Core/Module_Renderer.hpp"

namespace SpaceMarines
{

class Renderer;

namespace DebugDrawMode
{ enum List {
	None = 0,
	SystemOnly,
	SystemAndBullet
};}

class DebugDrawer : public btIDebugDraw
{
public:
	Shader* debugShader;
	DebugDrawer(Renderer* renderer);
	~DebugDrawer();
	void drawLine(const Vector3 &ptA, const Vector3 &ptB, const Vector3 &color = Vector3::ONE);
	void drawAxis(const Vector3 &point, const float size = 1.0f, const Quaternion &rotation = Quaternion::IDENTITY, const Vector3 &color = Vector3::ZERO);
	void setDrawMode(DebugDrawMode::List mode);
	//Bullet interface
	//{
		void drawLine(const btVector3 &ptA, const btVector3 &ptB, const btVector3 &color)
			{ drawLine(Vector3(ptA), Vector3(ptB), Vector3(color)); }
		void drawContactPoint(const btVector3 &ptA, const btVector3 &ptB, const float a, const int b, const btVector3 &color)
			{ drawAxis(ptA, a, Quaternion::IDENTITY, color); }
		void reportErrorWarning(const char* error)
			{ throw Exception(error); }
		void draw3dText(const btVector3 &pos, const char* text)
			{}
		void setDebugMode(int mode) { btDebugMode = mode;}
		int getDebugMode() const { return btDebugMode; }
	//}
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

	int btDebugMode;
	DebugDrawMode::List mode;

};

}
