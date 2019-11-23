#include "Sound.h"


namespace MT
{

	void Sound::onInit()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}

	void Sound::bindAsset(std::shared_ptr<Asset> asset)
	{
		soundContainer.bindAsset(asset);
	}

	void Sound::loadSoundClip(std::string path, std::string name)
	{
		soundContainer.loadSoundClip(path, name);
	}

	void Sound::playSoundClip(std::string name, double volume)
	{
		soundContainer.playSoundClip(name, volume);
	}

}
