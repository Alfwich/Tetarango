#include "TextureContainer.h"

namespace MT
{
	void TextureContainer::bindScreen(std::shared_ptr<Screen> screen)
	{
		this->screen = screen;
	}

	void TextureContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	bool TextureContainer::loadTexture(std::string path, std::string key)
	{
		if (textureCache.count(key) > 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to load Texture for key=" + key + ", path=" + path + ", texture already exists in the texture cache");
			return false;
		}

		textureCache[key] = std::make_unique<Texture>(path, screen, asset);
		Logger::instance()->log("TextureContainer::Loaded Texture key=" + key + ", path=" + path);
		return textureCache[key]->isLoaded();
	}

	std::shared_ptr<Texture> TextureContainer::getTexture(std::string key)
	{
		if (textureCache.count(key) == 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to find texture for key=" + key);
			return nullptr;
		}

		return textureCache[key];
	}

	std::shared_ptr<Texture> TextureContainer::getEmptyTextureTextForKey(std::string key)
	{
		if (textureCache.count(key) != 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to get empty TextureText for key=" + key + ", attempting to rebind existing texture");
			return nullptr;
		}

		return std::make_unique<TextureText>(screen, asset);
	}

	bool TextureContainer::hasTexture(std::string key)
	{
		return textureCache.count(key) > 0;
	}

	void TextureContainer::rebindAllTextures()
	{
		for (auto texture : textureCache)
		{
			texture.second->rebindTexture();
		}
	}
}
