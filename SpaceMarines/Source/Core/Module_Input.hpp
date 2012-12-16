#pragma once
#include "Prerequisites.hpp"
namespace SpaceMarines
{

namespace KeyCodes
{
enum codes
{
	Esc = GLFW_KEY_ESC,
	Enter = GLFW_KEY_ENTER,
	Up = GLFW_KEY_UP,
	Down = GLFW_KEY_DOWN,
	Left = GLFW_KEY_LEFT,
	Right = GLFW_KEY_RIGHT,
	Space = GLFW_KEY_SPACE
};
}

namespace Module
{

class Input
{
public:

	Input()
	{
		screenSize = Vector2(1, 1);
		mouseViewportPosition = Vector2(0, 0);
		lastMouseViewportPosition = Vector2(0, 0);
	}
	~Input()
	{

	}

	Vector2 screenSize;
	Vector2 lastMouseViewportPosition;
	Vector2 mouseViewportPosition;

	void pollMouse()
	{
		lastMouseViewportPosition = mouseViewportPosition;
		mouseViewportPosition = screenToViewport(getMousePos());
	}
	bool isKeyPressed(KeyCodes::codes code)
	{
		return (glfwGetKey(code) == GLFW_PRESS);
	}
	bool isKeyPressed(const char code)
	{
		return (glfwGetKey(code) == GLFW_PRESS);
	}
	Vector2 getMousePos()
	{
		static int x, y;
		glfwGetMousePos(&x, &y);
		return Vector2(x, y);
	}
	Vector2 getMouseViewportDelta()
	{
		return mouseViewportPosition - lastMouseViewportPosition;
	}
	Vector2 getMouseViewportPos()
	{
		return mouseViewportPosition;
	}
	bool isMouseButtonPressed(unsigned short index)
	{
		return (glfwGetMouseButton(index) == GLFW_PRESS);
	}
private:
	Vector2 screenToViewport(const Vector2 &pos)
	{
		return pos / screenSize * 2.0f - 1.0f;
	}


};

}
}
