#pragma once

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Element.h"
#include "ui/renderable/container/Container.h"
#include "engine/module/font/Font.h"
#include "util/Color.h"

namespace AW
{

	class Text : public Container
	{
		int fontSize = 0;
		std::string fontName;
		std::shared_ptr<Element> textPositioningElement;
		std::shared_ptr<Texture> cachedTextureText;

		std::string textureBindingKey;
		TextureTextRenderConfiguration config;
		void renderText();
		bool dynamicResizing = true;

	public:
		Text();
		Text(std::string fontName, int fontSize);
		~Text();

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

		std::shared_ptr<Texture> getTextureText();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onLayoutChildren();

		void setClipRect(AW::Rect rect);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};

}
