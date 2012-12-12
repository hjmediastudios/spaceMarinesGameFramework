/*
 * Renderer.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: nick
 */

#include "Core.hpp"
#include "Module_Renderer.hpp"
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glfw.h>

namespace SpaceMarines
{
namespace Module
{

Renderer::Renderer(const std::string &assetPath, const Vector2 &windowSize, const char *pipelineResource)
{
	this->assetPath = assetPath;
	this->screenSize = windowSize;
	this->pipelineFilePath = pipelineResource;
	pipeline = 0;
	this->camera = nullptr;
	_started = 0;
	this->debugDrawer = nullptr;
}

Renderer::~Renderer()
{
	delete debugDrawer;
	h3dRelease();
	glfwTerminate();
}

bool Renderer::init()
{
	if (!glfwInit()) throw Exception("Error initializing GLFW");
	if (!setupWindow()) throw Exception("Error opening window");
	if (glewInit() != GLEW_OK) throw Exception("GLEW couldn't be initialized");

	debugDrawer = new DebugDrawer(this);

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
	if (!glfwOpenWindow((int)screenSize.x, (int)screenSize.y, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return false;
	}
	glfwSwapInterval(0);
	return true;
}

Vector2 Renderer::getScreenSize() const
{
	return screenSize;
}

void Renderer::setCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* Renderer::getCamera()
{
	return camera;
}

void Renderer::update()
{
	h3dRender(camera->cameraNode);
	h3dFinalizeFrame();
	debugDrawer->render();
	Modules::gui().render();
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

}
} /* namespace SpaceMarines */
