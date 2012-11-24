#include "DebugDrawer.hpp"
namespace SpaceMarines
{

DebugDrawer::DebugDrawer(Renderer* renderer)
{
	this->renderer = renderer;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	debugShader = Shader::newShaderFromOneFile(renderer->getAssetPath() + "shaders/debugShader.glsl");
	Ldb_mvpMat = debugShader->getUniformLocation("mvpMat");
	if (Ldb_mvpMat == INVALID_UNIFORM_LOCATION)
		throw Exception("Error finding debug uniforms!");

	glGenBuffers(1, &LlinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, LlinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &LlinesIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LlinesIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	setDebugMode(0);
}

void DebugDrawer::render()
{
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
	indices.push_back((unsigned int) vertices.size());
	vertices.push_back(DebugVertex(ptA, color));
	indices.push_back((unsigned int) vertices.size());
	vertices.push_back(DebugVertex(ptB, color));
}

void DebugDrawer::drawAxis(const Vector3 &point, const float size, const Quaternion &rotation, const Vector3 &color)
{
	unsigned int centerIndex = vertices.size();
	indices.push_back(centerIndex);
	vertices.push_back(DebugVertex(point, (color == Vector3::ZERO ? Vector3::ONE : color)));
	indices.push_back(centerIndex + 1);
	vertices.push_back(DebugVertex(point + rotation * Vector3::RIGHT * size, (color == Vector3::ZERO ? Vector3::RIGHT : color)));
	indices.push_back(centerIndex);
	indices.push_back(centerIndex + 2);
	vertices.push_back(DebugVertex(point + rotation * Vector3::UP * size, (color == Vector3::ZERO ? Vector3::UP : color)));
	indices.push_back(centerIndex);
	indices.push_back(centerIndex + 3);
	vertices.push_back(DebugVertex(point + rotation * Vector3::FORWARD * size, (color == Vector3::ZERO ? Vector3::FORWARD : color)));

}

}
