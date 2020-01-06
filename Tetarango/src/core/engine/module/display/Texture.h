#pragma once

#include "GL/glew.h"

#include "engine/module/asset/ImageBundle.h"

namespace AW
{
	class Screen;
	class Asset;

	class Texture
	{
	private:
		std::shared_ptr<ImageBundle> bundle;
		std::string path, name;
		bool allowRebindWithRawPixelData = false;

	protected:
		std::shared_ptr<Screen> screen;
		std::shared_ptr<Asset> asset;

		unsigned int textureId = 0;
		int width = 0, height = 0;

		void bindOpenGLTexture(SDL_Surface* surface);

	public:
		Texture();
		Texture(unsigned int textureId, unsigned int width, unsigned int height);
		Texture(std::shared_ptr<Asset> asset);
		Texture(std::string path, std::shared_ptr<Asset> asset);
		virtual ~Texture();

		unsigned int openGlTextureId();

		int getWidth();
		int getHeight();
		virtual void rebind();
		bool isLoaded();

		void releaseTexture();
		void rebindTexture();
		void rebindWithEmptyData(unsigned int width, unsigned height);
		virtual void rebindWithImageBundle(std::shared_ptr<ImageBundle> bundle);
	};
}

#include "engine/module/display/Screen.h"
#include "engine/module/asset/Asset.h"
