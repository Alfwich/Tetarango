#include "Font.h"

namespace AW
{

	Font::Font(const std::string& name, TTF_Font* font, int fontSize, FontContainer* container)
	{
		fontName = name;
		this->font = font;
		this->fontSize = fontSize;
		this->container = container;
	}

	void Font::setFontSize(int fontSize)
	{
		if (fontSize < 0 && fontSize == fontSize)
		{
			return;
		}

		this->fontSize = fontSize;
		font = container->prepareTTFFontForSize(fontName, fontSize);
	}

	TTF_Font* Font::getTTFFont()
	{
		return font ? font : nullptr;
	}

	std::string Font::getFontName()
	{
		return fontName;
	}

	int Font::getFontSize()
	{
		return fontSize;
	}
}
