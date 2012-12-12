#include "BlokGUI.hpp"


namespace BlokGUI
{

const float Core::Font::FontScalingFactor = 1.0f;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Core::Font::Font(const char* fontPath, unsigned short size)
{
	font = new FTTextureFont(fontPath);
	if (font->Error())
		throw Exception("Error creating font");
	font->FaceSize(size*FontScalingFactor);
	scalingFactorInv = 1.0f / FontScalingFactor;
}

Core::Font::~Font()
{
	delete font;
}

void Core::Font::render(const char* text, float x, float y, unsigned int color, short alignment)
{
	glColor4ubv((GLubyte*) &color);

	float transX = 0.0f, transY = 0.0f;

	switch(alignment)
	{
	case 1:
		transX = x - font->Advance(text)*0.5f*scalingFactorInv;
		transY = y + font->FaceSize() * scalingFactorInv, 0.0f;
		break;
	default:
		transX = x;
		transY = y, 0.0f;
		break;
	}
	glTranslatef(transX, transY, 0.0f);
	glScalef(scalingFactorInv, -scalingFactorInv, scalingFactorInv);
	font->Render(text);
	glScalef(FontScalingFactor, -FontScalingFactor, FontScalingFactor);
	glTranslatef(-transX, -transY, 0.0f);
}

unsigned short Core::Font::height() const
{
	return font->FaceSize() * scalingFactorInv;
}

}
