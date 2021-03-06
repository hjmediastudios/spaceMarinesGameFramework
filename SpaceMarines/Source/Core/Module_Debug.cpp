#include "Module_Debug.hpp"
#include <GL/gl.h>
#include <GL/glew.h>

namespace SpaceMarines
{
namespace Module
{

DebugDrawer::DebugDrawer(Renderer* renderer)
{
	this->renderer = renderer;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	debugShader = BlokGL::Shader::NewShaderFromOneFile( (renderer->getAssetPath() + "shaders/debugShader.glsl").c_str() );
	Ldb_mvpMat = debugShader->getUniformLocation("mvpMat");
	if (Ldb_mvpMat == BlokShader::INVALID_UNIFORM)
		throw Exception("Error finding debug uniforms!");

	glGenBuffers(1, &LlinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, LlinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &LlinesIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LlinesIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	setDrawMode(DebugDrawMode::None);
}

void DebugDrawer::setDrawMode(DebugDrawMode::List mode)
{
	if (mode != DebugDrawMode::SystemAndBullet)
	{
		setDebugMode(btIDebugDraw::DBG_NoDebug);
	}
	else
	{
		setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
	}
	this->mode = mode;
}

void DebugDrawer::render()
{
	if (mode == DebugDrawMode::None) return;

	glPopAttrib();

	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (renderer->camera == nullptr) throw Exception("Can't DebugDraw without a camera");

	debugShader->enable();

	Matrix4 mvpMat = renderer->camera->getProjectionMatrix() * renderer->camera->getViewMatrix();
	glUniformMatrix4fv(Ldb_mvpMat, 1, GL_TRUE, mvpMat.ptr());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, LlinesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DebugVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (const GLvoid*) 12);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LlinesIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

	debugShader->disable();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	vertices.clear();
	indices.clear();
}

DebugDrawer::~DebugDrawer()
{
	glDeleteBuffers(1, &LlinesVBO);
	glDeleteBuffers(1, &LlinesIBO);
}

void DebugDrawer::drawLine(const Vector3 &ptA, const Vector3 &ptB, const Vector3 &color)
{
	if (mode == DebugDrawMode::None) return;

	indices.push_back((unsigned int) vertices.size());
	vertices.push_back(DebugVertex(ptA, color));
	indices.push_back((unsigned int) vertices.size());
	vertices.push_back(DebugVertex(ptB, color));
}

void DebugDrawer::drawAxis(const Vector3 &point, const float size, const Quaternion &rotation, const Vector3 &color)
{
	if (mode == DebugDrawMode::None) return;

	unsigned int centerIndex = vertices.size();

	if (color != Vector3::ZERO)
	{
		indices.push_back(centerIndex);
		vertices.push_back(DebugVertex(point, color));
		indices.push_back(centerIndex + 1);
		vertices.push_back(DebugVertex(point + rotation * Vector3::RIGHT * size, color));
		indices.push_back(centerIndex);
		indices.push_back(centerIndex + 2);
		vertices.push_back(DebugVertex(point + rotation * Vector3::UP * size, color));
		indices.push_back(centerIndex);
		indices.push_back(centerIndex + 3);
		vertices.push_back(DebugVertex(point + rotation * Vector3::FORWARD * size, color));
		return;
	}

	indices.push_back(centerIndex++);
	vertices.push_back(DebugVertex(point, Vector3::RIGHT));
	indices.push_back(centerIndex++);
	vertices.push_back(DebugVertex(point + rotation * Vector3::RIGHT * size, Vector3::RIGHT));

	indices.push_back(centerIndex++);
	vertices.push_back(DebugVertex(point, Vector3::UP));
	indices.push_back(centerIndex++);
	vertices.push_back(DebugVertex(point + rotation * Vector3::UP * size, Vector3::UP));

	indices.push_back(centerIndex++);
	vertices.push_back(DebugVertex(point, Vector3::FORWARD));
	indices.push_back(centerIndex++);
	vertices.push_back(DebugVertex(point + rotation * Vector3::FORWARD * size, Vector3::FORWARD));


}

}
}
