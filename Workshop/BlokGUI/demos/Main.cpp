#include <GL/glfw.h>
#include "../src/BlokGUI.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
	glfwInit();
	if (!glfwOpenWindow(1280, 720, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		throw BlokGUI::Exception("Error opening window");
	}
	glfwSwapInterval(0);
	BlokGUI::Core core(1280, 720);
	core.setSystemFont("/home/nick/git/spaceMarinesGameFramework/Workshop/BlokGUI/assets/fonts/DroidSans.ttf", 12);
	double time = glfwGetTime();
	double newTime = 0.0;


	int mouseX, mouseY, mouseScroll;
	while (!glfwGetKey(GLFW_KEY_ESC))
	{
		//Select input
		glfwGetMousePos(&mouseX, &mouseY);
		mouseScroll = glfwGetMouseWheel();

		core.beginFrame(mouseX, mouseY, mouseScroll, glfwGetMouseButton(0), glfwGetMouseButton(1));

		core.drawRoundedRect(590, 505, 10, 10, 5,
				core.drawButton("Button 0", 500, 500, 80, 25, BlokGUI::Color::Blue, true, BlokGUI::Color::White) ? BlokGUI::Color::Green : BlokGUI::Color::Red);

		core.drawRoundedRect(590, 535, 10, 10, 5,
						core.drawButton("Button 1", 500, 530, 80, 25, BlokGUI::Color::Orange, true, BlokGUI::Color::White) ? BlokGUI::Color::Green : BlokGUI::Color::Red);
		core.drawLine(600, 600, 800, 500, 2, BlokGUI::Color::LimeGreen);
		core.endFrame();

		glfwSwapBuffers();

		//Time
		newTime = glfwGetTime();
		float frameTime = ((float) newTime) - ((float) time);
		std::cout << 1.0f / (frameTime) << " fps\n";
		time = newTime;
	}
	glfwTerminate();
}
