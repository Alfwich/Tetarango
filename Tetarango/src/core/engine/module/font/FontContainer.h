#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"

namespace AWCore
{
	class Font;

	class FontPack 
	{
	public:
		FontPack(std::string path, TTF_Font* font, int fontSize) { this->path = path, this->fontSizeToTTFFont[fontSize] = font; }
		std::string path;
		std::unordered_map<int, TTF_Font*> fontSizeToTTFFont;
	};

	class FontContainer : public IBaseModule
	{
	protected:
		std::shared_ptr<Asset> asset;

		int defaultFontSize;
		std::unordered_map<std::string, std::shared_ptr<FontPack>> nameToFontPack;
		bool loadFont(std::string name, int fontSize);
		bool loadNewFont(std::string path, std::string name, int fontSize);
		void cleanupFont(TTF_Font* font);

	public:
		FontContainer();

		void bindAsset(std::shared_ptr<Asset> asset);

		void setDefaultFontSize(int fontSize);
		TTF_Font* prepareTTFFontForSize(std::string name, int fontSize);

		bool loadFont(std::string path, std::string name, int fontSize = 24);

		std::shared_ptr<Font> createFont(std::string name, int fontSize = -1);

		bool hasFont(std::string name);

	};
}

#include "Font.h"
