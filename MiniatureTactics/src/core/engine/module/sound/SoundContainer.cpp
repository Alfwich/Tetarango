#include "SoundContainer.h"


namespace MT
{
	void SoundContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	void SoundContainer::loadSoundClip(std::string path, std::string name)
	{
		if (soundClips.count(name) == 1)
		{
			Logger::instance()->logCritical("Sound::Failed to load sound path=" + path + ", with name=" + name);
			return;
		}

		auto clip = std::make_shared<SoundClip>(path, asset);
		soundClips[name] = clip;
		Logger::instance()->log("Sound::Loaded sound name= " + name + ", path=" + path);
	}

	void SoundContainer::playSoundClip(std::string name, double volume)
	{
		if (soundClips.count(name) == 0)
		{
			Logger::instance()->logCritical("Sound::Failed to play sound name=" + name + ", at volume=" + std::to_string(volume));
			return;
		}

		soundClips[name]->play(volume);
	}

}