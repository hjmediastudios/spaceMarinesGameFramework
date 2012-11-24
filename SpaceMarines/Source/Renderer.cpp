/*
 * Renderer.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: nick
 */

#include "Renderer.hpp"

namespace SpaceMarines
{

Renderer::Renderer(const std::string &assetPath, const Vector2 &windowSize, const char *pipelineResource)
{
	this->assetPath = assetPath;
	this->windowSize = windowSize;
	this->pipelineFilePath = pipelineResource;
	pipeline = 0;
	this->camera = nullptr;
	_started = 0;
#ifdef PP_DebugDraw
	Ldb_color = -1;
	Ldb_mvpMat = -1;
	LlinesVBO = -1;
	debugShader = nullptr;
#endif
}

Renderer::~Renderer()
{
#ifdef PP_DebugDraw
	delete debugShader;
#endif
	h3dRelease();
	glfwTerminate();
}

bool Renderer::init()
{
	if (!glfwInit()) throw Exception("Error initializing GLFW");
	if (!setupWindow()) throw Exception("Error opening window");

#ifdef PP_DebugDraw
	if (glewInit() != GLEW_OK) throw Exception("GLEW couldn't be initialized");
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	debugShader = Shader::newShaderFromOneFile(getAssetPath() + "shaders/debugShader.glsl");
	Ldb_mvpMat = debugShader->getUniformLocation("mvpMat");
	Ldb_color = debugShader->getUniformLocation("color");
	if (Ldb_color == INVALID_UNIFORM_LOCATION || Ldb_mvpMat == INVALID_UNIFORM_LOCATION)
		throw Exception("Error finding debug uniforms!");

    Vector3 vertices[3];
    vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
    vertices[1] = Vector3(1.0f, -1.0f, 0.0f);
    vertices[2] = Vector3(0.0f, 1.0f, 0.0f);
	glGenBuffers(1, &LlinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, LlinesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	if (!h3dInit()) throw Exception("Error initializing Horde3D");



	pipeline = h3dAddResource(H3DResTypes::Pipeline, pipelineFilePath.c_str(), 0);

	h3dSetOption(H3DOptions::FastAnimation, 0);

	if (!h3dutLoadResourcesFromDisk(assetPath.c_str()))
	{
		h3dutDumpMessages();
		throw Exception("Unable to load asset data.");
	}

	_started = 1;
	return true;
}

bool Renderer::loadResources()
{
	if (!h3dutLoadResourcesFromDisk(assetPath.c_str()))
		return false;
	return true;
}

void Renderer::start()
{
	if (_started != 1) throw Exception("Renderer must be initialized before it can be started");
	if (!loadResources()) {h3dutDumpMessages(); throw Exception("Unable to load resources"); }
	_started = 2;
	if (camera == nullptr) throw Exception("Renderer has no camera");
	if (_started != 2) throw Exception("Renderer hasn't been properly started.");
}

bool Renderer::setupWindow()
{
	if (!glfwOpenWindow((int)windowSize.x, (int)windowSize.y, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return false;
	}
	glfwSwapInterval(0);
	return true;
}

Vector2 Renderer::getWindowSize() const
{
	return windowSize;
}

void Renderer::setCamera(Camera* camera)
{
	this->camera = camera;
}

GameObject* Renderer::getCamera()
{
	return camera->gameObject;
}

void Renderer::update()
{
	h3dRender(camera->cameraNode);
	h3dFinalizeFrame();
#ifdef PP_DebugDraw
	debugDraw();
#endif

	// Finish rendering of frame
    glfwSwapBuffers();
}

std::string Renderer::getAssetPath() const
{
#ifndef WINDOWS
	return assetPath + "/";
#else
	return assetPath + "\\";
#endif
}

/***********************************
 *     Debug drawing functions
 ***********************************/

//Private draw method
#ifdef PP_DebugDraw

void Renderer::debugDraw()
{
	glPopAttrib();

	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (camera == nullptr) throw Exception("Can't DebugDraw without a camera");

	debugShader->enable();

	Matrix4 identity = camera->getProjectionMatrix() * camera->getViewMatrix();

	glUniformMatrix4fv(Ldb_mvpMat, 1, GL_TRUE, identity.ptr());

	glEnableVertexAttribArray(0);
	glUniform3f(Ldb_color, 1.0f, 1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, LlinesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	debugShader->disable();
	glDisableVertexAttribArray(0);
	std::cout << "Hey\n";
	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

#endif


} /* namespace SpaceMarines */
