#pragma once

#include "engine/module/asset/Asset.h"

namespace AW
{

	class SoundClip
	{
		Mix_Chunk* chunk = nullptr;
		std::shared_ptr<Asset> asset;

		void loadSound(std::string path);

	public:
		SoundClip(std::string soundPath, std::shared_ptr<Asset> asset);
		virtual ~SoundClip();

		Mix_Chunk* getChunk();
	};

}
