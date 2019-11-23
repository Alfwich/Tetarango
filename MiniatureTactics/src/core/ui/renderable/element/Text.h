#pragma once

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Element.h"
#include "engine/module/font/Font.h"
#include "util/Color.h"

namespace MT
{

	class Text : public Element
	{
		int fontSize = 0;
		std::string fontName;

		std::string textureBindingKey;
		TextureTextRenderConfiguration config;
		void renderText();
		bool dynamicResizing = true;

	public:
		Text();
		Text(std::string fontName, int fontSize);
		void setDynamicResizing(bool flag);

		void setFont(std::string name, int fontSize);
		void setFontSize(int fontSize);
		int getFontSize();

		void setTextRenderMode(TextRenderMode mode);

		void setTextColor(int r, int g, int b, int a = 0xff);

		void setBackgroundColor(int r, int g, int b, int a = 0xff);
		Color getBackgroundColor();

		void setText(std::string text);
		std::string getText();

		void onInitialAttach();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};

}
