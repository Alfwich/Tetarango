#pragma once

#include <string>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"
#include "engine/module/IBaseModule.h"

namespace AW
{

	class Screen;

	class TextureContainer : public IBaseModule
	{
		std::shared_ptr<Screen> screen;
		std::shared_ptr<Asset> asset;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;

	public:
		void bindScreen(std::shared_ptr<Screen> screen);
		void bindAsset(std::shared_ptr<Asset> asset);

		bool loadTexture(std::string path, std::string key);
		bool hasTexture(std::string key);
		void removeTexture(std::string key);

		std::shared_ptr<Texture> getTexture(std::string key);
		std::shared_ptr<Texture> getEmptyTextureForKey(std::string key);
		std::shared_ptr<Texture> getEmptyTextureTextForKey(std::string key);
		void rebindAllTextures();
	};

}

#include "engine/module/display/Screen.h"
