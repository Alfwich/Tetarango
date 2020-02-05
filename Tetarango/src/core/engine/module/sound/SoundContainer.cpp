#include "SoundContainer.h"

namespace AW
{
	void SoundContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	void SoundContainer::loadSoundClip(const std::string& path, const std::string& name)
	{
		if (shaders.count(name) == 1)
		{
			Logger::instance()->logCritical("Sound::Failed to load sound path=" + path + ", with name=" + name);
			return;
		}

		auto clip = std::make_shared<SoundClip>(path, asset);
		shaders[name] = clip;
		Logger::instance()->log("Sound::Loaded sound name= " + name + ", path=" + path);
	}

	std::shared_ptr<SoundClip> SoundContainer::getSoundClip(const std::string& name)
	{
		if (shaders.count(name) == 0)
		{
			Logger::instance()->logCritical("Sound::Failed to get sound name=" + name);
			return nullptr;
		}

		return shaders[name];
	}

	void SoundContainer::loadMusic(const std::string& path, const std::string& name)
	{
		if (musics.count(name) == 1)
		{
			Logger::instance()->logCritical("Sound::Failed to load sound path=" + path + ", with name=" + name);
			return;
		}

		auto clip = std::make_shared<Music>(path, asset);
		musics[name] = clip;
		Logger::instance()->log("Sound::Loaded sound name= " + name + ", path=" + path);
	}

	std::shared_ptr<Music> SoundContainer::getMusic(const std::string& name)
	{
		if (musics.count(name) == 0)
		{
			Logger::instance()->logCritical("Sound::Failed to get music name=" + name);
			return nullptr;
		}

		return musics[name];
	}

	void SoundContainer::cleanup()
	{
		shaders.clear();
		musics.clear();
	}
}