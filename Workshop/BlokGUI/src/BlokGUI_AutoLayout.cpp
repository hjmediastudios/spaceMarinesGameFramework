#include "BlokGUI.hpp"

namespace BlokGUIInternal
{
const unsigned int BlokGUIInternal::Core::defaultColor = rgba255(50, 50, 50, 200);
const unsigned int BlokGUIInternal::Core::defaultTextColor = rgba255(255, 255, 255, 225);

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
	drawRoundedRect(panelX, panelY, maxCols*0.5f*buttonWidth + padding*(maxCols+1), (rows+1)*buttonHeight + padding*(rows+2), 4, defaultColor, true);
	cols = 0;
	maxCols = 0;
	rows = 0;
}

bool Core::button(const char* text, bool continuous)
{
	int x, y;
	x = lastX + padding;
	y = lastY + padding;

	bool ret = drawButton(text, x, y, buttonWidth, buttonHeight, defaultColor, continuous, defaultTextColor);

	lastX = x + buttonWidth;
	cols += 2;
	return ret;
}

void Core::separatorLine()
{
	newRow();
	lastY = lastY + padding;
	drawLine(panelX, lastY + buttonHeight * 0.5f, maxCols*0.5f * buttonWidth + padding*(maxCols), lastY + buttonHeight * 0.5f, 3, defaultColor);
	lastY -= padding;
	newRow();
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
	drawSlider(x + 12, y + buttonHeight/4, 2*buttonWidth+padding - 24, buttonHeight/2, value, min, max, defaultTextColor, defaultColor);
	lastX = x + (2*buttonWidth) + padding;
	cols += 4;
}

void Core::checkBox(const char* text, bool* value)
{
	int x = lastX + padding, y = lastY + padding;
	drawCheckBox(x + halfButtonWidth - buttonHeight, y, buttonHeight, buttonHeight, value, defaultColor);
	drawText(x + halfButtonWidth - buttonHeight - 3, y - systemFont->height() * 0.5f, text, defaultTextColor, TextAlign::Right, true);
	lastX = x + halfButtonWidth;
	cols ++;
}

void Core::label(const char* text)
{
	int x = lastX + padding, y = lastY + padding;
	drawText(x + halfButtonWidth - 6, y - buttonHeight * 0.25f, text, defaultTextColor, TextAlign::Right, true);
	lastX = x + halfButtonWidth;
	cols++;
}


}
