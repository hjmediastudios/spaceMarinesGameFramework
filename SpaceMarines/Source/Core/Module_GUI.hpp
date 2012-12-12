#pragma once

#include "Prerequisites.hpp"
#include "Module_GUI_Math.hpp"
#include "Module_Debug.hpp"
#include "../Components/Camera.hpp"

#include <AntTweakBar.h>

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
	void addPanel(const char* name);
private:
	Renderer* renderer;
	float hey;
};

} /* namespace Module */
} /* namespace SpaceMarines */
