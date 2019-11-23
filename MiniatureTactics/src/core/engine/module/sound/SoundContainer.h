#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SoundClip.h"
#include "engine/module/logger/Logger.h"
#include "engine/module/asset/Asset.h"


namespace MT
{

	class SoundContainer
	{
		std::shared_ptr<Asset> asset;
		std::unordered_map<std::string, std::shared_ptr<SoundClip>> soundClips;
	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		void loadSoundClip(std::string path, std::string name);
		void playSoundClip(std::string name, double volume);
	};

}