#include <GL/glfw.h>
#include "../src/BlokGUI.hpp"
#include <iostream>
#include <sstream>

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
	float floatValue = 0.5f;
	float floatValue1 = 2.0f;
	bool checked = false;

	int mouseX, mouseY, mouseScroll;
	while (!glfwGetKey(GLFW_KEY_ESC))
	{
		//Select input
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetMousePos(&mouseX, &mouseY);
		mouseScroll = glfwGetMouseWheel();


		core.beginFrame(mouseX, mouseY, mouseScroll, glfwGetMouseButton(0), glfwGetMouseButton(1));

		core.beginPanel(10, 10);
		core.button("Howdy");
		if (core.button("Hey, boss!")) std::cout << "Hey\n";
		core.newRow();
		core.button("Howdy");
		core.button("Howdy");
		core.button("Howdy");
		core.newRow();
		core.label("Fig Speed");
		core.slider(&floatValue, 0.0f, 1.0f);

		core.endPanel();

		//Time
		newTime = glfwGetTime();
		float frameTime = ((float) newTime) - ((float) time);
		time = newTime;

		core.endFrame();
		glfwSwapBuffers();
	}
	glfwTerminate();
}
