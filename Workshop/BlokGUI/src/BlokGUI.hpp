#pragma once

#include "Main.hpp"
#include <FTGL/ftgl.h>

namespace BlokGUI
{

namespace CommandType
{enum List {
	Text,
	Rect
};}

class Core
{
public:
	Core(int windowX, int windowY);
	void beginFrame();
	void endFrame();
	bool setSystemFont(const char* fontPath, unsigned short size);

	void drawText(int x, int y, const char* text, unsigned int color);
	void drawRect(int x, int y, int w, int h, unsigned int color);
	void drawRoundedRect(int x, int y, int w, int h, int rounding, unsigned int color);

	inline unsigned int rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
	{
        return (r) | (g << 8) | (b << 16) | (a << 24);
	}

private:
	int windowX, windowY;
	float invWindowX, invWindowY;

	class Font
	{
	public:
		Font(const char* fontPath, unsigned short size);
		~Font();
		unsigned short height() const;
		void render(const char* text, float x, float y);
	private:
		FTFont* font;
	};

	Font* systemFont;
private:
	struct Rect
	{
		short x, y, w, h, rounding;
	};
	struct Line
	{
		short x0, y0, x1, y1, radius;
	};
	struct Text
	{
		short x, y, align;
		const char* text;
	};

	struct Command
	{
		char type;
		unsigned int color;
		union
		{
			Rect rect;
			Line line;
			Text text;
		};
	};

	static const unsigned int maxTextPoolSize = 8000;
	char textPool[maxTextPoolSize];
	unsigned int textPoolSize;
	const char* allocateText(const char* text);

	static const unsigned int maxCommandQueueSize = 4000;
	Command commandQueue[maxCommandQueueSize];
	unsigned int commandQueueSize;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#define TEMP_COORD_COUNT 100
	static float g_tempCoords[TEMP_COORD_COUNT*2];
	static float g_tempNormals[TEMP_COORD_COUNT*2];

	static const int NUM_CIRCLE_VERTS = 8*4;
	float circleVerts[NUM_CIRCLE_VERTS*2];
	#define PI 3.14159265f

	//Rendering functions
	void renderGUI();

	void renderText(int x, int y, const char* text);
	void renderRect(float x, float y, float w, float h, float fth, unsigned int color);
	void renderRoundedRect(float x, float y, float w, float h, float r, float fth, unsigned int color);

	void renderPolygon(const float* coords, unsigned int numCoords, float r, unsigned int color);

	float aspect;
	float depth;
};

}
