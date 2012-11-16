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
}

Renderer::~Renderer()
{
	h3dRelease();
	glfwTerminate();
}

bool Renderer::init()
{
	if (!glfwInit()) throw Exception("Error initializing GLFW");
	if (!setupWindow()) throw Exception("Error opening window");
	if (!h3dInit()) throw Exception("Error initializing Horde3D");

	pipeline = h3dAddResource(H3DResTypes::Pipeline, pipelineFilePath.c_str(), 0);

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
	if (!loadResources()) throw Exception("Unable to load resources");
	_started = 2;
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

void Renderer::update()
{
	if (_started != 2) throw Exception("Renderer hasn't been properly started.");
	h3dRender(camera->cam);

    // Finish rendering of frame
    h3dFinalizeFrame();
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


} /* namespace SpaceMarines */
