#include "BlokGUI.hpp"

namespace BlokGUI
{

void Core::renderText(int x, int y, const char* text)
{
	if (!systemFont) return;
	systemFont->render(text, x*invWindowX - 1.0f, y*invWindowY + 1.0f - (systemFont->height()*invWindowY));
}

void Core::renderPolygon(const float* coords, unsigned int numCoords, float fth, unsigned int color)
{
	glColor4ubv((GLubyte*) &color);
	glBegin(GL_TRIANGLE_FAN);

	for (unsigned int i = 0; i < numCoords; i += 2)
	{
		glVertex2f(coords[2*i + 0], coords[2*i + 1]);
	}
	glEnd();
}

void Core::renderRect(float x, float y, float w, float h, float fth, unsigned int color)
{
	glColor4ubv((GLubyte*) &color);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);

	glEnd();
}

void Core::renderRoundedRect(float x, float y, float w, float h, float r, float fth, unsigned int color)
{
	float rx = r;
	float ry = r;

    const unsigned n = NUM_CIRCLE_VERTS/4;
	float verts[(n+1)*4*2];
	const float* cverts = circleVerts;
	float* v = verts;

	for (unsigned i = 0; i <= n; ++i)
	{
			*v++ = x+w-rx + cverts[i*2]*rx;
			*v++ = y+h-ry + cverts[i*2+1]*ry;
	}

	for (unsigned i = n; i <= n*2; ++i)
	{
			*v++ = x+rx + cverts[i*2]*rx;
			*v++ = y+h-ry + cverts[i*2+1]*ry;
	}

	for (unsigned i = n*2; i <= n*3; ++i)
	{
			*v++ = x+rx + cverts[i*2]*rx;
			*v++ = y+ry + cverts[i*2+1]*ry;
	}

	for (unsigned i = n*3; i < n*4; ++i)
	{
			*v++ = x+w-rx + cverts[i*2]*rx;
			*v++ = y+ry + cverts[i*2+1]*ry;
	}
	*v++ = x+w-rx + cverts[0]*rx;
	*v++ = y+ry + cverts[1]*ry;

	renderPolygon(verts, (n+1)*4, fth, color);
}

void Core::renderGUI()
{
	//Prep for 2D rendering
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, windowX, windowY, 0);
	//Enable alpha blending
	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	for (unsigned int i=0; i < commandQueueSize; i++)
	{
		Command &cmd = commandQueue[i];
		switch (cmd.type)
		{
		case CommandType::Rect:
			if (cmd.rect.rounding < 1)
				renderRect(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h, 1.0f, cmd.color);
			else
				renderRoundedRect(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h, cmd.rect.rounding, 1.0f, cmd.color);
			break;
		default:
			break;
		}
	}

}

}
