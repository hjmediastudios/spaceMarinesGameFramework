#include "BlokGUI.hpp"

namespace BlokGUIInternal
{

void Core::renderText(int x, int y, const char* text, unsigned int color, short alignment, char flags)
{
	if (!systemFont) return;
	systemFont->render(text, x, y + (flags == 0 ? 0 : systemFont->height()/2), color, alignment);
}

void Core::renderPolygon(const float* coords, unsigned int numCoords, float fth, unsigned int color)
{
	if (numCoords > TEMP_COORD_COUNT)
		numCoords = TEMP_COORD_COUNT;

	for (unsigned i = 0, j = numCoords - 1; i < numCoords; j = i++)
	{
		const float* v0 = &coords[j * 2];
		const float* v1 = &coords[i * 2];
		float dx = v1[0] - v0[0];
		float dy = v1[1] - v0[1];
		float d = sqrtf(dx * dx + dy * dy);
		if (d > 0)
		{
			d = 1.0f / d;
			dx *= d;
			dy *= d;
		}
		tempNormals[j * 2 + 0] = dy;
		tempNormals[j * 2 + 1] = -dx;
	}

	for (unsigned i = 0, j = numCoords - 1; i < numCoords; j = i++)
	{
		float dlx0 = tempNormals[j * 2 + 0];
		float dly0 = tempNormals[j * 2 + 1];
		float dlx1 = tempNormals[i * 2 + 0];
		float dly1 = tempNormals[i * 2 + 1];
		float dmx = (dlx0 + dlx1) * 0.5f;
		float dmy = (dly0 + dly1) * 0.5f;
		float dmr2 = dmx * dmx + dmy * dmy;
		if (dmr2 >= 0.25f)
		{
			float scale = 1.0f / dmr2;
			if (scale > 1.5f)
				scale = 1.5f;
			dmx *= scale;
			dmy *= scale;
		}
		tempCoords[i * 2 + 0] = coords[i * 2 + 0] + dmx * fth;
		tempCoords[i * 2 + 1] = coords[i * 2 + 1] + dmy * fth;
	}

	unsigned int colTrans = rgba255(0, 0, 0, 0);

	glBegin(GL_TRIANGLES);

	glColor4ubv((GLubyte*) &color);

	for (unsigned i = 0, j = numCoords - 1; i < numCoords; j = i++)
	{
		glVertex2fv(&coords[i * 2]);
		glVertex2fv(&coords[j * 2]);
		glColor4ubv((GLubyte*) &colTrans);
		glVertex2fv (&tempCoords[j * 2]);

		glVertex2fv(&tempCoords[j * 2]);
		glVertex2fv(&tempCoords[i * 2]);

		glColor4ubv((GLubyte*) &color);
		glVertex2fv(&coords[i * 2]);
	}

	glColor4ubv((GLubyte*) &color);
	for (unsigned i = 2; i < numCoords; ++i)
	{
		glVertex2fv(&coords[0]);
		glVertex2fv(&coords[(i - 1) * 2]);
		glVertex2fv(&coords[i * 2]);
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

void Core::renderLine(float x0, float y0, float x1, float y1, float w, float fth, unsigned int color)
{
	//StartHere GUI
    float dx = x1-x0;
    float dy = y1-y0;
    float d = sqrtf(dx*dx+dy*dy);
    if (d > 0.0001f)
    {
            d = 1.0f/d;
            dx *= d;
            dy *= d;
    }
    float nx = dy;
    float ny = -dx;
    float verts[4*2];
    w -= fth;
    w *= 0.5f;
    if (w < 0.01f) w = 0.01f;
    dx *= w;
    dy *= w;
    nx *= w;
    ny *= w;

    verts[0] = x0-dx-nx;
    verts[1] = y0-dy-ny;

    verts[2] = x0-dx+nx;
    verts[3] = y0-dy+ny;

    verts[4] = x1+dx+nx;
    verts[5] = y1+dy+ny;

    verts[6] = x1+dx-nx;
    verts[7] = y1+dy-ny;

    renderPolygon(verts, 4, fth, color);
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
	glOrtho(0, windowX, windowY, 0, 0.0f, 10.0f);
	//Enable alpha blending
	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (std::list<Command>::iterator it = commandQueue.begin(); it != commandQueue.end(); it++)
	{
		Command &cmd = *it;
		switch (cmd.type)
		{
		case CommandType::Rect:
			if (cmd.rect.rounding < 1)
				renderRect(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h, 1.25f, cmd.color);
			else
				renderRoundedRect(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h, cmd.rect.rounding, 1.25f, cmd.color);
			break;
		case CommandType::Text:
			renderText(cmd.text.x, cmd.text.y, cmd.text.text, cmd.color, cmd.text.align, cmd.flags);
			break;
		case CommandType::Line:
			renderLine(cmd.line.x0, cmd.line.y0, cmd.line.x1, cmd.line.y1, cmd.line.radius, 1.25f, cmd.color);
			break;
		default:
			break;
		}
	}

	glDisable(GL_BLEND);
}

}
