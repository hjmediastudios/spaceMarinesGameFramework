#pragma once

#include "Main.hpp"
#include <FTGL/ftgl.h>
#include <list>

namespace BlokGUIInternal
{

namespace CommandType
{enum List {
	Text,
	Rect,
	Line
};}

namespace TextAlign
{enum List{
	Left = 0,
	Center = 1,
	Right = 2
};}

inline unsigned int rgba255(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
{
    return (r) | (g << 8) | (b << 16) | (a << 24);
}

inline unsigned int rgba1(float r, float g, float b, float a = 1.0f)
{
	return rgba255((unsigned char) r*255, (unsigned char) g*255, (unsigned char) b*255, (unsigned char) a*255);
}

template<typename T>
const char* toString(const T &item)
{
	std::ostringstream os;
	if (!(os << item))
		return "";
	else
		return os.str().c_str();
}

namespace Color
{
	static const unsigned int White = rgba255(244, 244, 244, 244);
	static const unsigned int Black = rgba255(1, 1, 1, 255);
	static const unsigned int Red = rgba255(222, 0, 13, 255);
	static const unsigned int Blue = rgba255(0, 87, 168, 255);
	static const unsigned int Yellow = rgba255(254, 196, 0, 255);
	static const unsigned int Green = rgba255(0, 123, 40, 255);
	static const unsigned int MediumGreen = rgba255(0, 150, 36, 255);
	static const unsigned int LimeGreen = rgba255(149, 185, 11, 255);
	static const unsigned int Orange = rgba255(231, 99, 24, 255);
	static const unsigned int LightGray = rgba255(156, 146, 145, 255);
	static const unsigned int DarkGray = rgba255(76, 81, 86, 255);
}

class Core
{
	//Public API-exposable functions
public:
	Core(int windowX, int windowY);
	void beginFrame(int mouseX, int mouseY, int scroll, bool mouseL, bool mouseR);
	void endFrame();
	bool setSystemFont(const char* fontPath, unsigned short size);

	bool drawButton(const char* text, int x, int y, int w, int h, unsigned int color, bool continuousActivation = false, unsigned int textColor = Color::White);

	void drawText(int x, int y, const char* text, unsigned int color = Color::White, TextAlign::List alignment = TextAlign::Left, bool alignVCenter = false);
	void drawRect(int x, int y, int w, int h, unsigned int color = Color::DarkGray, bool atBack = false);
	void drawRoundedRect(int x, int y, int w, int h, int rounding, unsigned int color = Color::DarkGray, bool atBack = false);
	void drawLine(int x0, int y0, int x1, int y1, int width, unsigned int color = Color::LightGray);
	void drawSlider(int x, int y, int w, int h, float* value, float valMin = 0.0f, float valMax = 1.0f, unsigned int sliderColor = Color::LightGray, unsigned int barColor = Color::DarkGray);
	void drawVSlider(int x, int y, int w, int h, float* value, float valMin = 0.0f, float valMax = 1.0f, unsigned int sliderColor = Color::LightGray, unsigned int barColor = Color::DarkGray);
	void drawCheckBox(int x, int y, int w, int h, bool* value, unsigned int color = rgba255(150, 150, 150, 140));

///////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	int windowX, windowY;
	float invWindowX, invWindowY;

	class Font
	{
	public:
		static const float FontScalingFactor;
		Font(const char* fontPath, unsigned short size);
		~Font();
		float height() const;
		void render(const char* text, float x, float y, unsigned int color, short alignment);
	private:
		FTFont* font;
		float scalingFactorInv;
		float heightf;
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
		char flags;
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
	std::list<Command> commandQueue;
	unsigned int commandQueueSize;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Internal rendering functions

	#define TEMP_COORD_COUNT 100
	float tempCoords[TEMP_COORD_COUNT*2];
	float tempNormals[TEMP_COORD_COUNT*2];

	static const int NUM_CIRCLE_VERTS = 8*4;
	float circleVerts[NUM_CIRCLE_VERTS*2];
	#define PI 3.14159265f

	//Rendering functions
	void renderGUI();

	void renderText(int x, int y, const char* text, unsigned int color, short alignment, char flags);
	void renderRect(float x, float y, float w, float h, float fth, unsigned int color);
	void renderRoundedRect(float x, float y, float w, float h, float r, float fth, unsigned int color);
	void renderLine(float x0, float y0, float x1, float y1, float w, float fth, unsigned int color);
	void renderPolygon(const float* coords, unsigned int numCoords, float r, unsigned int color);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct
	{
		int mouseX;
		int mouseY;
		int scroll;

		bool mouseL, mouseLPressed, mouseLReleased;
		bool mouseR, mouseRPressed, mouseRReleased;

	} input;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Internal logic functions

	unsigned int activeID;
	unsigned int lastActiveID;
	bool isMouseInRect(int x, int y, int w, int h);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Auto-layout functions

	unsigned short buttonHeight;
	unsigned short halfButtonWidth;
	unsigned short buttonWidth;
	unsigned short padding;
	static const unsigned int defaultColor;
	static const unsigned int defaultTextColor;
	bool addRight;

	unsigned short panelX;
	unsigned short panelY;

	unsigned short maxX;
	unsigned short maxY;

	unsigned short lastY;
	unsigned short lastX;

public:
	void beginPanel(int x, int y);
	bool button(const char* text, bool enabled = true);
	void slider(float* value, float min, float max);
	void label(const char* text);
	void checkBox(const char* text, bool* value);
	void separatorLine();
	void newRow();
	void endPanel();

};

}
