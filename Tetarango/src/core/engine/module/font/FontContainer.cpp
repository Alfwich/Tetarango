#include "FontContainer.h"

namespace AW
{
	FontContainer::FontContainer()
	{
		defaultFontSize = 24;
	}

	void FontContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	void FontContainer::setDefaultFontSize(int fontSize)
	{
		if (fontSize < 1)
		{
			return;
		}

		defaultFontSize = fontSize;
		for (const auto fontPack : nameToFontPack)
		{
			prepareTTFFontForSize(fontPack.first, fontSize);
		}
	}

	void FontContainer::cleanupFont(TTF_Font* font)
	{
		TTF_CloseFont(font);
	}

	bool FontContainer::loadFont(const std::string& name, int fontSize)
	{
		std::shared_ptr<FontPack> fontPack = nameToFontPack[name];

		if (fontPack->fontSizeToTTFFont.count(fontSize) == 1)
		{
			return true;
		}

		TTF_Font* font;

		const auto ttfFont = nameToFontPack.at(name)->pack;
		if (ttfFont != nullptr)
		{
			SDL_RWops* rw = SDL_RWFromMem(ttfFont->data.get(), ttfFont->size);
			font = TTF_OpenFontRW(rw, 1, fontSize);
		}
		else
		{
			font = TTF_OpenFont(fontPack->path.c_str(), fontSize);
		}


		if (!font)
		{
			Logger::instance()->logCritical("FontContainer::Failed to load font name=" + name + ", size=" + std::to_string(fontSize));
			return false;
		}

		fontPack->fontSizeToTTFFont[fontSize] = font;
		Logger::instance()->log("FontContainer::Loaded font name=" + name + ", size=" + std::to_string(fontSize));

		return true;
	}

	bool FontContainer::loadNewFont(const std::string& path, const std::string& name, int fontSize)
	{
		TTF_Font* font;

		const auto ttfFont = asset->getAssetBundle(path);
		if (ttfFont != nullptr)
		{
			SDL_RWops* rw = SDL_RWFromMem(ttfFont->data.get(), ttfFont->size);
			font = TTF_OpenFontRW(rw, 1, fontSize);
		}
		else
		{
			font = TTF_OpenFont(path.c_str(), fontSize);
		}

		if (!font)
		{
			Logger::instance()->logCritical("FontContainer::Failed to load font name=" + name + ", size=" + std::to_string(fontSize));
			return false;
		}

		nameToFontPack[name] = std::make_unique<FontPack>(path, font, fontSize, ttfFont);

		Logger::instance()->log("FontContainer::Loaded font name=" + name + ", size=" + std::to_string(fontSize) + ", path=" + path);

		return true;
	}

	bool FontContainer::loadFont(const std::string& path, const std::string& name, int fontSize)
	{
		if (nameToFontPack.count(name) == 1)
		{
			return loadFont(name, fontSize);
		}

		bool loaded = loadNewFont(path, name, fontSize);

		if (loaded && fontSize != defaultFontSize)
		{
			prepareTTFFontForSize(name, defaultFontSize);
		}

		return loaded;
	}

	TTF_Font* FontContainer::prepareTTFFontForSize(const std::string& name, int fontSize)
	{
		if (nameToFontPack.count(name) == 0)
		{
			Logger::instance()->logCritical("FontContainer::Failed to acquire font name=" + name + ", size=" + std::to_string(fontSize) + ". This was not found in the FontContainer.");
			return nullptr;
		}

		std::shared_ptr<FontPack> fontPack = nameToFontPack[name];

		if (fontSize > 0 && fontPack->fontSizeToTTFFont.count(fontSize) == 0)
		{
			loadFont(name, fontSize);
		}

		return fontSize > 0
			? fontPack->fontSizeToTTFFont[fontSize]
			: fontPack->fontSizeToTTFFont[defaultFontSize];
	}

	std::shared_ptr<Font> FontContainer::createFont(const std::string& name, int fontSize)
	{
		TTF_Font* font = prepareTTFFontForSize(name, fontSize);

		if (!font)
		{
			return nullptr;
		}

		return std::make_unique<Font>(name, font, fontSize, this);
	}

	bool FontContainer::hasFont(const std::string& name)
	{
		return nameToFontPack.count(name) == 1;
	}

	void FontContainer::onBindLuaHooks(const std::shared_ptr<Lua>& lua)
	{
		lua->registerBoundFunction("loadFont", shared_from_this());
		lua->registerBoundFunction("createFont", shared_from_this());
	}

	void FontContainer::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == "loadFont" && obj->args.size() == 2) loadFont("res/font/" + obj->args[0], obj->args[1]);
		else if (func == "createFont" && obj->args.size() == 1) createFont(obj->args[0]);
		else if (func == "createFont" && obj->args.size() == 2) createFont(obj->args[0], std::stoi(obj->args[1]));
	}
}
