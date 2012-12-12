#include <GL/glfw.h>
#include "../src/BlokGUI.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
	glfwInit();
	if (!glfwOpenWindow(1280, 720, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
		throw BlokGUI::Exception("Error opening window");
	BlokGUI::Core core(1280, 720);
	core.setSystemFont("/home/nick/workspace/BlokGUI/assets/fonts/neogen.ttf", 24);
	while (!glfwGetKey(GLFW_KEY_ESC))
	{
		core.beginFrame();
		core.drawRect(25, 25, 150, 50, core.rgba(255, 100, 0));
		core.drawRoundedRect(5, 5, 200, 50, 25, core.rgba(255, 255, 150, 150));
//		core.drawText(0, 0, "Hey", 0);
		core.endFrame();
		glfwSwapBuffers();
	}
}
