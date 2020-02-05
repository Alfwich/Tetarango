#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"

namespace AW
{
	class Font;

	class FontPack
	{
	public:
		FontPack(const std::string& path, TTF_Font* font, int fontSize, const std::shared_ptr<ResourceBundle> ttfFont)
			: path(path), pack(ttfFont)
		{
			this->fontSizeToTTFFont[fontSize] = font;
		}

		const std::string path;
		const std::shared_ptr<ResourceBundle> pack;

		std::unordered_map<int, TTF_Font*> fontSizeToTTFFont;
	};

	class FontContainer : public IBaseModule
	{
	protected:
		std::shared_ptr<Asset> asset;

		int defaultFontSize;
		std::unordered_map<std::string, std::shared_ptr<FontPack>> nameToFontPack;
		bool loadFont(const std::string& name, int fontSize);
		bool loadNewFont(const std::string& path, const std::string& name, int fontSize);
		void cleanupFont(TTF_Font* font);

	public:
		FontContainer();

		void bindAsset(std::shared_ptr<Asset> asset);

		void setDefaultFontSize(int fontSize);
		TTF_Font* prepareTTFFontForSize(const std::string& name, int fontSize);

		bool loadFont(const std::string& path, const std::string& name, int fontSize = 24);

		std::shared_ptr<Font> createFont(const std::string& name, int fontSize = -1);

		bool hasFont(const std::string& name);

	};
}

#include "Font.h"
