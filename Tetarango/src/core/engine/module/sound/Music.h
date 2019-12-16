#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "engine/module/logger/Logger.h"
#include "util/StringHelper.h"
#include "engine/module/asset/Asset.h"

namespace AWCore
{
	class Music
	{
		Mix_Music* music = nullptr;
		std::shared_ptr<Asset> asset;

		void loadMusic(std::string path);

	public:
		Music(std::string soundPath, std::shared_ptr<Asset> asset);
		virtual ~Music();

		Mix_Music* getMusic();
	};
}

