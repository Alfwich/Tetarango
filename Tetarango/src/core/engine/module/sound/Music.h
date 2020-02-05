#pragma once

#include "engine/module/asset/Asset.h"

namespace AW
{
	class Music
	{
		Mix_Music* music = nullptr;
		std::shared_ptr<Asset> asset;

		void loadMusic(std::string path);

	public:
		Music(const std::string& soundPath, std::shared_ptr<Asset> asset);
		virtual ~Music();

		Mix_Music* getMusic();
	};
}

