#include "Module_GUI.hpp"
#include <GL/gl.h>
#include <GL/glew.h>

namespace SpaceMarines
{
namespace Module
{

GUI::GUI(Renderer* renderer)
{
	if (renderer == nullptr) throw Exception("Attempted to create a GUI module with a null renderer");
	this->renderer = renderer;
	hey = 0.0f;
}

GUI::~GUI()
{
}

bool GUI::init()
{
	if (!renderer) throw Exception("Attempting to create a GUI without a renderer.");


	return true;
}

void GUI::addPanel(const char* name)
{

}


void GUI::render(int mouseX, int mouseY, bool isMouseClicked)
{
	BlokGUI::BeginFrame(mouseX, mouseY, isMouseClicked);
	BlokGUI::EndFrame();
}


} /* namespace Module */
} /* namespace SpaceMarines */
