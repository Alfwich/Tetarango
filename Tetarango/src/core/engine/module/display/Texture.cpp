#include "Texture.h"

namespace AW
{
	Texture::Texture()
	{
		allowRebindWithRawPixelData = true;
	}

	Texture::Texture(unsigned int textureId, unsigned int width, unsigned int height)
	{
		this->textureId = textureId;
		this->width = width;
		this->height = height;
	}

	Texture::Texture(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	Texture::Texture(std::string path, std::shared_ptr<Asset> asset)
	{
		this->path = path;
		this->asset = asset;
		rebindTexture();
	}

	Texture::~Texture()
	{
		releaseTexture();
	}

	unsigned int Texture::openGlTextureId()
	{
		return textureId;
	}

	void Texture::rebind()
	{
		if (allowRebindWithRawPixelData)
		{
			rebindWithImageBundle(bundle);
		}
		else
		{
			rebindTexture();
		}
	}

	void Texture::releaseTexture()
	{
		if (textureId != 0)
		{
			glDeleteTextures(1, &textureId);
			textureId = 0;
		}
	}

	void Texture::rebindTexture()
	{
		if (asset == nullptr || allowRebindWithRawPixelData) {
			return;
		}

		SDL_Surface* tempSurface = nullptr;

		const auto bitmap = asset->getAssetBundle(path);
		if (bitmap != nullptr)
		{
			SDL_RWops* rw = SDL_RWFromMem(bitmap->data.get(), bitmap->size);
			tempSurface = IMG_LoadPNG_RW(rw);
		}
		else
		{
			tempSurface = IMG_Load(path.c_str());
		}

		if (!tempSurface)
		{
			Logger::instance()->logCritical("Texture::Failed to render SDL surface for path=" + path);
			SDL_FreeSurface(tempSurface);
			return;
		}

		bindOpenGLTexture(tempSurface);

		width = tempSurface->w;
		height = tempSurface->h;
		SDL_FreeSurface(tempSurface);
	}

	void Texture::rebindWithEmptyData(unsigned int width, unsigned height)
	{
		if (textureId != 0)
		{
			glDeleteTextures(1, &textureId);
			textureId = 0;
		}

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		this->width = width;
		this->height = height;
	}

	void Texture::rebindWithImageBundle(std::shared_ptr<ImageBundle> bundle)
	{
		if (!allowRebindWithRawPixelData)
		{
			Logger::instance()->logCritical("Texture::Attempted to rebindWithRawPixelData with texture that forbids it");
			return;
		}

		if (bundle == nullptr || bundle->width == 0 || bundle->height == 0 || bundle->type == ImageBundleType::Unspecificed)
		{
			return;
		}

		SDL_Surface* tempSurface = nullptr;

		switch (bundle->type)
		{
		case ImageBundleType::Png:
		{
			SDL_RWops* rw = SDL_RWFromMem(&bundle->data[0], (int)bundle->data.size());
			tempSurface = IMG_LoadPNG_RW(rw);
		}
		break;

		case ImageBundleType::Raw:
			tempSurface = SDL_CreateRGBSurfaceWithFormatFrom(&bundle->data[0], bundle->width, bundle->height, 24, 3, SDL_PIXELFORMAT_RGB24);
			break;
		}

		if (!tempSurface)
		{
			Logger::instance()->logCritical("Texture::Failed to render SDL surface for image bundle");
			SDL_FreeSurface(tempSurface);
			return;
		}

		bindOpenGLTexture(tempSurface);

		width = tempSurface->w;
		height = tempSurface->h;

		SDL_FreeSurface(tempSurface);
	}

	void Texture::bindOpenGLTexture(SDL_Surface* tempSurface)
	{
		if (textureId != 0)
		{
			glDeleteTextures(1, &textureId);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (tempSurface->format->BitsPerPixel == 32)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempSurface->w, tempSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempSurface->pixels);
		}
		else if (tempSurface->format->BitsPerPixel == 24)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tempSurface->w, tempSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tempSurface->pixels);
		}
		else
		{
			Logger::instance()->logCritical("Texture::OpenGL::Failed to load raw image for path=" + path);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int Texture::getWidth()
	{
		return width;
	}

	int Texture::getHeight()
	{
		return height;
	}

	bool Texture::isLoaded()
	{
		return textureId != 0;
	}
}
