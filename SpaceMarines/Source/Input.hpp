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

namespace Input
{
static bool isKeyPressed(KeyCodes::codes code)
{
	return (glfwGetKey(code) == GLFW_PRESS);
}
static bool isKeyPressed(const char code)
{
	return (glfwGetKey(code) == GLFW_PRESS);
}


}

}
