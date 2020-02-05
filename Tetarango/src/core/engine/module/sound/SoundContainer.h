#pragma once

#include "SoundClip.h"
#include "Music.h"
#include "engine/module/asset/Asset.h"

namespace AW
{

	class SoundContainer
	{
		std::shared_ptr<Asset> asset;
		std::unordered_map<std::string, std::shared_ptr<SoundClip>> shaders;
		std::unordered_map<std::string, std::shared_ptr<Music>> musics;

	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		void loadSoundClip(const std::string& path, const std::string& name);
		std::shared_ptr<SoundClip> getSoundClip(const std::string& name);

		void loadMusic(const std::string& path, const std::string& name);
		std::shared_ptr<Music> getMusic(const std::string& name);

		void cleanup();
	};

}