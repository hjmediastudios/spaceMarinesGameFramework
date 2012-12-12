#include "BlokGUI.hpp"

namespace BlokGUI
{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Core::Font::Font(const char* fontPath, unsigned short size)
{
	font = new FTPixmapFont(fontPath);
	if (font->Error())
		throw Exception("Error creating font");
	font->FaceSize(size);
}

Core::Font::~Font()
{
	delete font;
}

void Core::Font::render(const char* text, float x, float y)
{
	glRasterPos2f(x, y);
	font->Render(text);
}

unsigned short Core::Font::height() const
{
	return font->FaceSize() + font->Ascender() * 0.5f;
}

}
