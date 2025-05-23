#include "TextureContainer.h"

namespace AW
{
	void TextureContainer::bindScreen(std::shared_ptr<Screen> screen)
	{
		this->screen = screen;
	}

	void TextureContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	bool TextureContainer::loadTexture(const std::string& path, const std::string& key)
	{
		if (textureCache.count(key) > 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to load Texture for key=" + key + ", path=" + path + ", texture already exists in the texture cache");
			return false;
		}

		textureCache[key] = std::make_shared<Texture>(path, asset);
		Logger::instance()->log("TextureContainer::Loaded Texture key=" + key + ", path=" + path);

		return textureCache[key]->isLoaded();
	}

	std::shared_ptr<Texture> TextureContainer::getTexture(const std::string& key)
	{
		if (textureCache.count(key) == 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to find texture for key=" + key);
			return nullptr;
		}

		return textureCache[key];
	}

	std::shared_ptr<Texture> TextureContainer::getEmptyTextureForKey(const std::string& key)
	{
		if (textureCache.count(key) != 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to get empty TextureText for key=" + key + ", attempting to rebind existing texture");
			return nullptr;
		}

		textureCache[key] = std::make_shared<Texture>();

		return textureCache[key];

	}

	std::shared_ptr<Texture> TextureContainer::getEmptyTextureTextForKey(const std::string& key)
	{
		if (textureCache.count(key) != 0)
		{
			Logger::instance()->logCritical("TextureContainer::Failed to get empty TextureText for key=" + key + ", attempting to rebind existing texture");
			return nullptr;
		}

		textureCache[key] = std::make_shared<TextureText>(asset);

		return textureCache[key];
	}

	bool TextureContainer::hasTexture(const std::string& key)
	{
		return textureCache.count(key) > 0;
	}

	void TextureContainer::removeTexture(const std::string& key)
	{
		if (hasTexture(key))
		{
			textureCache.erase(key);
		}
	}

	void TextureContainer::releaseAllTextures()
	{
		Logger::instance()->log("TextureContainer::Releasing " + std::to_string(textureCache.size()) + " textures");

		for (const auto textureNameToTexture : textureCache)
		{
			textureNameToTexture.second->releaseTexture();
		}
	}

	void TextureContainer::rebindAllTextures()
	{
		Logger::instance()->log("TextureContainer::Rebinding " + std::to_string(textureCache.size()) + " textures");

		for (const auto textureNameToTexture : textureCache)
		{
			textureNameToTexture.second->rebind();
		}
	}

	void TextureContainer::onBindLuaHooks(const std::shared_ptr<Lua>& lua)
	{
		lua->registerBoundFunction("loadTexture", shared_from_this());
	}

	void TextureContainer::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == "loadTexture" && obj->args.size() == 2) loadTexture("res/image/" + obj->args[0], obj->args[1]);
	}
}
