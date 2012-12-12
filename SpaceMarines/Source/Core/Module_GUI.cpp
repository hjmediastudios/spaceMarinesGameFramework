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
	TwTerminate();
}

bool GUI::init()
{
	if (!renderer) throw Exception("Attempting to create a GUI without a renderer.");
	TwInit(TW_OPENGL, nullptr);
	TwWindowSize(renderer->getScreenSize().x, renderer->getScreenSize().y);


	return true;
}

void GUI::addPanel(const char* name)
{
	TwBar* tweakBar;
	tweakBar = TwNewBar(name); //TODO GUIPanel object
	TwAddVarRW(tweakBar, "Variable", TW_TYPE_FLOAT, &hey, "");
}


void GUI::render()
{
	TwDraw();
}


} /* namespace Module */
} /* namespace SpaceMarines */
