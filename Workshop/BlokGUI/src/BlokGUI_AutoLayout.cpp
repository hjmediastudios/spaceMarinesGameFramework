#include "BlokGUI.hpp"

namespace BlokGUI
{
const unsigned int BlokGUI::Core::defaultColor = rgba255(150, 200, 255, 100);

void Core::beginPanel(int x, int y)
{
	panelX = x;
	panelY = y;
	lastY = panelY;
	lastX = panelX;
	addRight = true;
	maxX = lastX;
	maxY = lastY;
}

static int cols = 0;
static int rows = 0;
static int maxCols = 0;

void Core::endPanel()
{
	if (cols >= maxCols) maxCols = cols;
	drawRoundedRect(panelX, panelY, maxCols*0.5f*buttonWidth + padding*(maxCols+1), (rows+1)*buttonHeight + padding*(rows+2), 4, defaultColor);
	cols = 0;
	maxCols = 0;
	rows = 0;
}

bool Core::button(const char* text, bool enabled)
{
	int x, y;
	x = lastX + padding;
	y = lastY + padding;

	bool ret = drawButton(text, x, y, buttonWidth, buttonHeight, defaultColor);

	lastX = x + buttonWidth;
	cols += 2;
	return ret;
}

void Core::newRow()
{
	addRight = false;
	lastX = panelX;
	lastY += buttonHeight + padding;
	maxY += buttonHeight * 1.5f + padding;
	if (cols >= maxCols) maxCols = cols;
	cols = 0;
	rows++;
	maxX -= buttonWidth;
}

void Core::slider(float* value, float min, float max)
{
	int x, y;
	x = lastX + padding;
	y = lastY + padding;
	drawSlider(x + 12, y + buttonHeight/4, 2*buttonWidth+padding - 24, buttonHeight/2, value, min, max, defaultColor, defaultColor);
	lastX = x + (2*buttonWidth) + padding;
	cols += 4;
}

void Core::label(const char* text)
{
	int x = lastX + padding, y = lastY + padding;
	drawText(x + halfButtonWidth - 6, y, text, defaultColor, TextAlign::Right, true);
	lastX = x + halfButtonWidth;
	cols++;
}


}
