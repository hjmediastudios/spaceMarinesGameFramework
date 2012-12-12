#include "BlokGUI.hpp"
#include <string.h>
#include <iostream>
#include <math.h>

namespace BlokGUI
{



Core::Core(int windowX, int windowY)
{
	this->windowX = windowX;
	this->windowY = windowY;
	invWindowX = 1.0f / ((float) windowX);
	invWindowY = 1.0f / ((float) windowY);

	systemFont = nullptr;
	textPoolSize = 0;
	commandQueueSize = 0;
	depth = 0.0f;
	aspect = ((float) windowX) / ((float) windowY);

    for (int i = 0; i < NUM_CIRCLE_VERTS; ++i)
    {
            float a = (float)i/(float)NUM_CIRCLE_VERTS * PI *2;
            circleVerts[i*2+0] = cosf(a);
            circleVerts[i*2+1] = sinf(a);
    }

}

bool Core::setSystemFont(const char* fontPath, unsigned short size)
{
	systemFont = new Font(fontPath, size);
	return (systemFont != nullptr);
}

void Core::beginFrame()
{
//	glOrtho(0, windowX, windowY, 0, 0, 1000);
}

void Core::endFrame()
{
	renderGUI();
	//Render

	commandQueueSize = 0;
	textPoolSize = 0;
	glDisable(GL_BLEND);
}

const char* Core::allocateText(const char* text)
{
	unsigned int len = strlen(text);
	if (textPoolSize + len >= maxTextPoolSize) return 0;
	char* dst = &textPool[textPoolSize];
	memcpy(dst, text, len);
	textPoolSize += len;
	return dst;
}

void Core::drawText(int x, int y, const char* text, unsigned int color)
{
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Text;
	cmd.text.x = (short) x;
	cmd.text.y = (short) y;
	cmd.color = color;
	cmd.text.text = allocateText(text);
}
void Core::drawRect(int x, int y, int w, int h, unsigned int color)
{
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Rect;
	cmd.rect.x = (short) x;
	cmd.rect.y = (short) y;
	cmd.rect.w = (short) w;
	cmd.rect.h = (short) h;
	cmd.color = color;
	cmd.rect.rounding = 0;
}
void Core::drawRoundedRect(int x, int y, int w, int h, int rounding, unsigned int color)
{
	if (commandQueueSize >= maxCommandQueueSize) return;
	Command &cmd = commandQueue[commandQueueSize++];
	cmd.type = CommandType::Rect;
	cmd.rect.x = (short) x;
	cmd.rect.y = (short) y;
	cmd.rect.w = (short) w;
	cmd.rect.h = (short) h;
	cmd.rect.rounding = rounding;
	cmd.color = color;
}


}
