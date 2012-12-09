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

	Lvbo = 0;
	Libo = 0;
	guiShader = nullptr; //TODO shader
}

GUI::~GUI()
{
}

bool GUI::init()
{
	glGenBuffers(1, &Lvbo);
	glBindBuffer(GL_ARRAY_BUFFER, Lvbo);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &Libo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Libo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

void GUI::render()
{
}

void GUI::drawRectangle()
{

}

} /* namespace Module */
} /* namespace SpaceMarines */
