#include "Music.h"

namespace AW
{
	Music::Music(const std::string& path, std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
		loadMusic(path);
	}

	Music::~Music()
	{
		if (music != nullptr)
		{
			Mix_FreeMusic(music);
			music = nullptr;
		}
	}

	void Music::loadMusic(std::string path)
	{
		Mix_Music* music;

		const auto sound = asset->getAssetBundle(path);
		if (sound != nullptr)
		{
			SDL_RWops* rw = SDL_RWFromMem(sound->data.get(), sound->size);
			music = Mix_LoadMUS_RW(rw, 1);
		}
		else
		{
			music = Mix_LoadMUS(path.c_str());
		}

		if (music == nullptr)
		{
			Logger::instance()->logCritical("Music::Failed to load music on path=" + path);
			return;
		}

		this->music = music;
	}

	Mix_Music* Music::getMusic()
	{
		return music;
	}
}