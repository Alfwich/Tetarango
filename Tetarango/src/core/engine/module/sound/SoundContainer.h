#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SoundClip.h"
#include "Music.h"
#include "engine/module/logger/Logger.h"
#include "engine/module/asset/Asset.h"


namespace AWCore
{

	class SoundContainer
	{
		std::shared_ptr<Asset> asset;
		std::unordered_map<std::string, std::shared_ptr<SoundClip>> soundClips;
		std::unordered_map<std::string, std::shared_ptr<Music>> musics;

	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		void loadSoundClip(std::string path, std::string name);
		std::shared_ptr<SoundClip> getSoundClip(std::string name);

		void loadMusic(std::string path, std::string name);
		std::shared_ptr<Music> getMusic(std::string name);

		void cleanup();
	};

}