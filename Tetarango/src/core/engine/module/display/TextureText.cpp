#include "TextureText.h"

namespace AWCore
{
	TextureText::~TextureText()
	{
		if (textureId != 0)
		{
			glDeleteTextures(1, &textureId);
		}
	}

	void TextureText::updateConfiguration(TextureTextRenderConfiguration config)
	{
		this->config = config;
	}

	int TextureText::getOriginalWidth()
	{
		return ogWidth;
	}

	int TextureText::getOriginalHeight()
	{
		return ogHeight;
	}

	void TextureText::rebind()
	{
		rebindText(config);
	}

	void TextureText::rebindText(TextureTextRenderConfiguration textRenderConfiguration)
	{
		if (config.font == nullptr)
		{
			return;
		}

		SDL_Surface* tempSurface;

		const auto textToRender = config.text.empty() ? " " : config.text;
		switch (textRenderConfiguration.renderMode)
		{
		case TextRenderMode::SlowBackground:
			tempSurface = TTF_RenderText_Shaded(config.font->getTTFFont(), textToRender.c_str(), config.textColor.asSdlColor(), config.backgroundColor.asSdlColor());
			break;

		case TextRenderMode::SlowAlpha:
			tempSurface = TTF_RenderText_Blended(config.font->getTTFFont(), textToRender.c_str(), config.textColor.asSdlColor());
			break;

		case TextRenderMode::Fast:
		default:
			tempSurface = TTF_RenderText_Solid(config.font->getTTFFont(), textToRender.c_str(), config.textColor.asSdlColor());
			break;
		}

		if (!tempSurface)
		{
			Logger::instance()->logCritical("Text::Failed to render SDL surface for fontName=" + config.font->getFontName() + ", text=" + config.text);
			SDL_FreeSurface(tempSurface);
			return;
		}

		bindOpenGLTexture(tempSurface, textRenderConfiguration);

		ogWidth = tempSurface->w;
		ogHeight = tempSurface->h;

		SDL_FreeSurface(tempSurface);
	}

	void TextureText::bindOpenGLTexture(SDL_Surface* tempSurface, const TextureTextRenderConfiguration& textRenderConfiguration)
	{
		if (textureId != 0)
		{
			glDeleteTextures(1, &textureId);
		}

		const auto oldTextureId = textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (oldTextureId != 0)
		{

		}

		switch (textRenderConfiguration.renderMode)
		{
		case TextRenderMode::Fast:
		case TextRenderMode::SlowBackground:
		{
			// TODO: Look into more efficent method of displaying fast/background text
			auto data = std::make_unique<GLuint[]>(tempSurface->w * tempSurface->h);
			auto ptr = data.get();
			for (int y = 0; y < tempSurface->h; ++y)
			{
				for (int x = 0; x < tempSurface->w; ++x)
				{
					Uint8 *target_pixel = (Uint8 *)tempSurface->pixels + y * tempSurface->pitch + x * sizeof *target_pixel;
					auto color = &tempSurface->format->palette->colors[*target_pixel];
					auto adjustedPtr = ptr + x + (y * tempSurface->w);
					*adjustedPtr = color->a << 24 | color->r << 16 | color->g << 8 | color->b;
				}
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempSurface->w, tempSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());

			width = tempSurface->w;
			height = tempSurface->h;
		}
		break;

		case TextRenderMode::SlowAlpha:
		{
			const auto isTexturePowerOf2 = std::ceil(std::log2(tempSurface->w)) == std::floor(std::log2(tempSurface->w)) && std::ceil(std::log2(tempSurface->h)) == std::floor(std::log2(tempSurface->w));
			if (isTexturePowerOf2)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempSurface->w, tempSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempSurface->pixels);
			}
			else
			{
				const auto nextPow2Width = (int)std::pow(2, std::ceil(std::log2(tempSurface->w)));
				const auto nextPow2Height = (int)std::pow(2, std::ceil(std::log2(tempSurface->h)));
				const auto newSurface = SDL_CreateRGBSurfaceWithFormat(0, nextPow2Width, nextPow2Height, 32, SDL_PIXELFORMAT_BGRA32);
				auto destRect = SDL_Rect();
				destRect.x = (nextPow2Width - tempSurface->w) / 2;
				destRect.y = (nextPow2Height - tempSurface->h) / 2;
				destRect.w = tempSurface->w;
				destRect.h = tempSurface->h;
				SDL_BlitSurface(tempSurface, NULL, newSurface, &destRect);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newSurface->w, newSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, newSurface->pixels);

				width = newSurface->w;
				height = newSurface->h;

				SDL_FreeSurface(newSurface);
			}
		}
		break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
