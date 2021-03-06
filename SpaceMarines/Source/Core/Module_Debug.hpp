#pragma once
#include "Prerequisites.hpp"
#include "Module_Renderer.hpp"
#include "Module_GUI_Math.hpp"
#include <BlokGL.hpp>
namespace SpaceMarines
{

namespace DebugDrawMode
{ enum List {
	None = 0,
	SystemOnly,
	SystemAndBullet
};}

namespace Module
{

class Renderer;

class DebugDrawer : public btIDebugDraw
{
public:

	BlokGL::Shader* debugShader;
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



	std::vector<DebugVertex> vertices;
	std::vector<unsigned int> indices;

	int btDebugMode;
	DebugDrawMode::List mode;

};

}
}
