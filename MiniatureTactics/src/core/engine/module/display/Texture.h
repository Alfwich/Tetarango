#pragma once

#include <SDL.h>
#include "GL/glew.h"
#include <string>

namespace MT
{
	class Screen;
	class Asset;

	class Texture
	{
	private:
		std::string path;

	protected:
		std::shared_ptr<Screen> screen;
		std::shared_ptr<Asset> asset;

		SDL_Texture* texture;
		GLuint textureId = 0;
		int width = 0, height = 0;

		void bindOpenGLTexture(SDL_Surface* surface);

	public:
		Texture(std::shared_ptr<Screen> screen, std::shared_ptr<Asset> asset);
		Texture(std::string path, std::shared_ptr<Screen> screen, std::shared_ptr<Asset> asset);
		virtual ~Texture();

		SDL_Texture* raw();
		GLuint openGlTextureId();

		int getWidth();
		int getHeight();
		virtual void rebind();
		void rebindTexture();
		bool isLoaded();
	};
}

#include "engine/module/display/Screen.h"
#include "engine/module/asset/Asset.h"
