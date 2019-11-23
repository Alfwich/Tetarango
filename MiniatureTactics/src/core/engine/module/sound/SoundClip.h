#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "engine/module/logger/Logger.h"
#include "util/StringHelper.h"
#include "engine/module/asset/Asset.h"

namespace MT
{

	class SoundClip
	{
		Mix_Chunk* chunk;
		std::shared_ptr<Asset> asset;

		void loadSound(std::string path);

	public:
		SoundClip(std::string soundPath, std::shared_ptr<Asset> asset);
		virtual ~SoundClip();

		void play(double volume);
	};

}
