#pragma once

#include "Texture.h"
#include "engine/module/font/Font.h"
#include "util/Color.h"

namespace MT
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
		TextureTextRenderConfiguration config;

		void bindOpenGLTexture(SDL_Surface* surface, const TextureTextRenderConfiguration& textRenderConfiguration);

	public:
		TextureText(std::shared_ptr<Screen> screen, std::shared_ptr<Asset> asset) : Texture(screen, asset) {};
		TextureText(TextureTextRenderConfiguration config, std::shared_ptr<Screen> screen, std::shared_ptr<Asset> asset) : TextureText(screen, asset) { this->config = config; };
		~TextureText();

		void updateConfiguration(TextureTextRenderConfiguration config);

		void rebind();
		void rebindText(TextureTextRenderConfiguration textRenderConfiguration);
	};

}
