#pragma once

#include "engine/module/display/Screen.h"

namespace AW
{

	class FontContainer;

	class Font
	{
		TTF_Font* font;
		int fontSize;
		std::string fontName;
		FontContainer* container;
	public:
		Font(const std::string& name, TTF_Font* font, int fontSize, FontContainer* container);

		void setFontSize(int fontSize);
		TTF_Font* getTTFFont();

		std::string getFontName();
		int getFontSize();
	};
}

#include "FontContainer.h"
