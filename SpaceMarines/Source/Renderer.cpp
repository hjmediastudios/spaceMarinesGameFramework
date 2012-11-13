/*
 * Renderer.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: nick
 */

#include "Renderer.hpp"

namespace SpaceMarines
{

Renderer::Renderer(const std::string &assetPath, const Vector2 &windowSize)
{
	this->assetPath = assetPath;
	this->windowSize = windowSize;
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

	return true;
}

bool Renderer::setupWindow()
{
	if (!glfwOpenWindow(windowSize.x, windowSize.y, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
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


} /* namespace SpaceMarines */
