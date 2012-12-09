#pragma once

#include "Prerequisites.hpp"
#include "Module_GUI_Math.hpp"
#include "Module_Debug.hpp"
#include "../Components/Camera.hpp"

namespace SpaceMarines
{
namespace Module
{

class GUI
{
public:
	GUI(Renderer* renderer);
	virtual ~GUI();
	void render();
	bool init();
private:
	Shader* guiShader;
	void drawRectangle();
	Renderer* renderer;

	GLuint Lvbo;
	GLuint Libo;
	std::vector<DebugVertex> vertices;
	std::vector<unsigned int> indices;
};

} /* namespace Module */
} /* namespace SpaceMarines */
