#include "BlokGUI.hpp"
#include <string.h>
#include <iostream>
#include <math.h>

namespace BlokGUI
{

//Command queue and setup functions

Core::Core(int windowX, int windowY)
{
	this->windowX = windowX;
	this->windowY = windowY;
	invWindowX = 1.0f / ((float) windowX);
	invWindowY = 1.0f / ((float) windowY);

	systemFont = nullptr;
	textPoolSize = 0;
	commandQueueSize = 0;

    for (int i = 0; i < NUM_CIRCLE_VERTS; ++i)
    {
            float a = (float)i/(float)NUM_CIRCLE_VERTS * PI *2;
            circleVerts[i*2+0] = cosf(a);
            circleVerts[i*2+1] = sinf(a);
    }

    input.mouseX = 0;
    input.mouseY = 0;
    input.scroll = 0;
    input.mouseL = false; input.mouseLPressed = false; input.mouseLReleased = false;
    input.mouseR = false; input.mouseRPressed = false; input.mouseRReleased = false;

    activeID = 0;
    lastActiveID = 0;

    buttonHeight = 24;
    buttonWidth = buttonHeight * 6;
    halfButtonWidth = buttonHeight * 3;
    padding = 4;

    maxX = 0;
    maxY = 0;
    lastX = 0;
}

bool Core::setSystemFont(const char* fontPath, unsigned short size)
{
	systemFont = new Font(fontPath, size);
//    buttonHeight = systemFont->height();
//    buttonWidth = buttonWidth * 6;
	return (systemFont != nullptr);
}

void Core::beginFrame(int mouseX, int mouseY, int scroll, bool mouseL, bool mouseR)
{
//	glOrtho(0, windowX, windowY, 0, 0, 1000);
	input.mouseX = mouseX;
	input.mouseY = mouseY;
	input.scroll = scroll;

	input.mouseLPressed = !input.mouseL && mouseL;
	input.mouseLReleased = input.mouseL && !mouseL;
	input.mouseL = mouseL;

	input.mouseRPressed = !input.mouseR && mouseR;
	input.mouseRReleased = input.mouseR && !mouseR;
	input.mouseR = mouseR;
}

void Core::endFrame()
{
	renderGUI();
	//Render

	if (!input.mouseL)
		lastActiveID = 0;

	activeID = 0;
	commandQueueSize = 0;
	textPoolSize = 0;
	glDisable(GL_BLEND);
}

const char* Core::allocateText(const char* text)
{
	unsigned int len = strlen(text) + 1;
	if (textPoolSize + len >= maxTextPoolSize) return 0;
	char* dst = &textPool[textPoolSize];
	memcpy(dst, text, len);
	textPoolSize += len;
	return dst;
}

void Core::drawText(int x, int y, const char* text, unsigned int color, TextAlign::List alignment, bool alignVCenter)
{
	activeID++;
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Text;
	cmd.text.x = (short) x;
	cmd.text.y = (short) y;
	cmd.text.align = (short) alignment;
	cmd.flags = (char) (alignVCenter ? 1 : 0);
	cmd.color = color;
	cmd.text.text = allocateText(text);
}
void Core::drawRect(int x, int y, int w, int h, unsigned int color)
{
	activeID++;
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Rect;
	cmd.flags = 0;
	cmd.rect.x = (short) x;
	cmd.rect.y = (short) y;
	cmd.rect.w = (short) w;
	cmd.rect.h = (short) h;
	cmd.color = color;
	cmd.rect.rounding = 0;
}
void Core::drawRoundedRect(int x, int y, int w, int h, int rounding, unsigned int color)
{
	activeID++;
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Rect;
	cmd.flags = 0;
	cmd.rect.x = (short) x;
	cmd.rect.y = (short) y;
	cmd.rect.w = (short) w;
	cmd.rect.h = (short) h;
	cmd.rect.rounding = rounding;
	cmd.color = color;
}

void Core::drawLine(int x0, int y0, int x1, int y1, int width, unsigned int color)
{
	activeID++;
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Line;
	cmd.flags = 0;
	cmd.line.x0 = (short) x0;
	cmd.line.y0 = (short) y0;
	cmd.line.x1 = (short) x1;
	cmd.line.y1 = (short) y1;
	cmd.line.radius = width;
	cmd.color = color;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Logic functions

bool Core::isMouseInRect(int x, int y, int w, int h)
{
	return (input.mouseX >= x && input.mouseX <= (x + w) && input.mouseY >= y && input.mouseY <= (y + h));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public drawing functions

bool Core::drawButton(const char* text, int x, int y, int w, int h, unsigned int color, bool continuousActivation, unsigned int textColor)
{
	activeID++;
	drawRoundedRect(x, y, w, h, 5, color);
	drawText(x + w*0.5f, y+ h/2 - systemFont->height()/2, text, textColor, TextAlign::Center);

	bool ret = false;
	if (continuousActivation)
	{
		ret = input.mouseL && (isMouseInRect(x, y, w, h) || lastActiveID == activeID);
	}
	else
	{
		ret = (isMouseInRect(x, y, w, h) && input.mouseLReleased);
	}
	if (ret) lastActiveID = activeID;
	return ret;
}

void Core::drawSlider(int x, int y, int w, int h, float* value, float valMin, float valMax, unsigned int sliderColor, unsigned int barColor)
{
	activeID++;
	float sliderMargin = h / 8.0f;
	float sliderWidth = w / 10.0f;

	*value = clamp(*value, valMin, valMax);
	float barFactor = (*value - valMin) / (valMax - valMin);

	float sliderCenter = clamp(x + barFactor*w, x + sliderWidth*0.5f, x + w - sliderWidth*0.5f);
	float sliderX = clamp(sliderCenter - sliderWidth*0.5f, x, x + w - sliderWidth);

	drawRoundedRect(x, y, w, h, 4, barColor);
	drawRoundedRect(clamp(sliderX, x, x + w - sliderWidth), y, sliderWidth, h, 4, sliderColor);

	float halfHeight = h * 0.5f;
	drawText(x - 3, y - halfHeight, toString(valMin), barColor, TextAlign::Right, true);
	drawText(x + w + 3, y - halfHeight, toString(valMax), barColor, TextAlign::Left, true);

	if (input.mouseL && (isMouseInRect(sliderX, y, sliderWidth, h) || lastActiveID == activeID))
	{
		lastActiveID = activeID;
		*value = clamp(*value + (input.mouseX - sliderCenter) / ((float)w) * (valMax - valMin), valMin, valMax);
		drawText(sliderCenter, y - systemFont->height() - 2, toString(*value), sliderColor, TextAlign::Center);
	}
}

void Core::drawVSlider(int x, int y, int w, int h, float* value, float valMin, float valMax, unsigned int sliderColor, unsigned int barColor)
{
	activeID++;
	float sliderMargin = w / 8.0f;
	float sliderHeight = h / 10.0f;
	float halfWidth = x + w/2;
	*value = clamp(*value, valMin, valMax);
	float barFactor = (*value - valMin) / (valMax - valMin);

	float sliderY = y + h - barFactor*h;
	float sliderCenter = sliderY + sliderHeight*0.5f;

	drawRoundedRect(x+sliderMargin, y, w-sliderMargin, h, 4, barColor);
	drawRoundedRect(x, clamp(sliderY, y, y + h - sliderHeight), w, sliderHeight, 4, sliderColor);

	drawText(halfWidth, y - systemFont->height() - 2, toString(valMax), barColor, TextAlign::Center, true);
	drawText(halfWidth, y + h, toString(valMin), barColor, TextAlign::Center, true);

	if (input.mouseL && (isMouseInRect(x, sliderY, w, sliderHeight) || lastActiveID == activeID))
	{
		lastActiveID = activeID;
		*value = clamp(*value + (sliderCenter - input.mouseY) / ((float)h) * (valMax - valMin), valMin, valMax);
	}
}

void Core::drawCheckBox(int x, int y, int w, int h, bool* value, unsigned int color)
{
	activeID++;
	float margin = w/4.0f;
	float margin2 = 2.0f * margin;
	drawRoundedRect(x, y, w, h, 4, color);

	if (isMouseInRect(x, y, w, h) && input.mouseLReleased)
		*value = !(*value);
	if (*value)
		drawRoundedRect(x + margin, y + margin, w - margin2 + 1, h - margin2 + 1, 4, rgba255(180, 180, 180, 255));
}

}
