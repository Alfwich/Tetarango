#include "SoundClip.h"


namespace MT
{


	SoundClip::SoundClip(std::string path, std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
		loadSound(path);
	}

	SoundClip::~SoundClip()
	{
		if (chunk != nullptr)
		{
			Mix_FreeChunk(chunk);
		}
	}

	void SoundClip::loadSound(std::string path)
	{
		Mix_Chunk* chunk;

		const auto sound = asset->getAssetBundle(path);
		if (sound != nullptr)
		{
			SDL_RWops* rw = SDL_RWFromMem(sound->data.get(), sound->size);
			chunk = Mix_LoadWAV_RW(rw, 1);
		}
		else
		{
			chunk = Mix_LoadWAV(path.c_str());
		}

		if (chunk == nullptr)
		{
			Logger::instance()->logCritical("Sound::Failed to load sound on path=" + path);
			return;
		}

		this->chunk = chunk;
	}

	void SoundClip::play(double volume)
	{
		if (chunk != nullptr)
		{
			int vol = (int)(128 * volume);
			//Mix_Volume(-1, vol);
			Mix_VolumeChunk(chunk, vol);
			Mix_PlayChannel(-1, chunk, 0);
		}
	}

}