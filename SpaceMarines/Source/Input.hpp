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

class Input
{
protected:
	static Vector2 screenSize;
	friend class Application;
public:
	static bool isKeyPressed(KeyCodes::codes code)
	{
		return (glfwGetKey(code) == GLFW_PRESS);
	}
	static bool isKeyPressed(const char code)
	{
		return (glfwGetKey(code) == GLFW_PRESS);
	}
	static Vector2 getMousePos()
	{
		static int x, y;
		glfwGetMousePos(&x, &y);
		return Vector2(x, y);
	}
	static Vector2 getMousePosNormalized()
	{
		return getMousePos() / screenSize * 2.0f - 1.0f;
	}


};

}
