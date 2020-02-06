#pragma once

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

		bool loadTexture(const std::string& path, const std::string& key);
		bool hasTexture(const std::string& key);
		void removeTexture(const std::string& key);

		std::shared_ptr<Texture> getTexture(const std::string& key);
		std::shared_ptr<Texture> getEmptyTextureForKey(const std::string& key);
		std::shared_ptr<Texture> getEmptyTextureTextForKey(const std::string& key);

		void releaseAllTextures();
		void rebindAllTextures();

		virtual void onBindLuaHooks(const std::shared_ptr<Lua>& lua) override;

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override { return "texture"; };
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};

}

#include "engine/module/display/Screen.h"
