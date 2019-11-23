#include "Texture.h"
namespace MT
{

	Texture::Texture(std::shared_ptr<Screen> screen, std::shared_ptr<Asset> asset)
	{
		this->screen = screen;
		this->asset = asset;
	}

	Texture::Texture(std::string path, std::shared_ptr<Screen> screen, std::shared_ptr<Asset> asset)
	{
		this->path = path;
		this->screen = screen;
		this->asset = asset;
		rebindTexture();
	}

	Texture::~Texture()
	{
		if (texture != nullptr)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}

		if (textureId != 0)
		{
			glDeleteTextures(0, &textureId);
			textureId = 0;
		}
	}

	SDL_Texture* Texture::raw()
	{
		return texture;
	}

	GLuint Texture::openGlTextureId()
	{
		return textureId;
	}

	void Texture::rebind()
	{
		rebindTexture();
	}

	void Texture::rebindTexture()
	{
		SDL_Surface* tempSurface;

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

	void Texture::bindOpenGLTexture(SDL_Surface* tempSurface)
	{
		if (textureId != 0)
		{
			glDeleteTextures(1, &textureId);
		}

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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
		return texture != nullptr || textureId != 0;
	}
}
