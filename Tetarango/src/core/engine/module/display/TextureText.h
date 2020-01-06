#pragma once

#include "Texture.h"
#include "engine/module/font/Font.h"

namespace AW
{

	enum class TextRenderMode
	{
		Fast,
		SlowBackground,
		SlowAlpha
	};

	class TextureTextRenderConfiguration
	{
	public:
		TextRenderMode renderMode;
		std::shared_ptr<Font> font;
		std::string text;
		Color textColor;
		Color backgroundColor;
	};

	class TextureText : public Texture
	{
		int originalW = 0, originalH = 0;
		TextureTextRenderConfiguration config;

		void bindOpenGLTexture(SDL_Surface* surface, const TextureTextRenderConfiguration& textRenderConfiguration);

	public:
		TextureText(std::shared_ptr<Asset> asset) : Texture(asset) {};
		TextureText(TextureTextRenderConfiguration config, std::shared_ptr<Asset> asset) : TextureText(asset) { this->config = config; };
		~TextureText();

		void updateConfiguration(TextureTextRenderConfiguration config);
		int getOriginalWidth();
		int getOriginalHeight();

		void rebind();
		void rebindText(TextureTextRenderConfiguration textRenderConfiguration);
	};

}
