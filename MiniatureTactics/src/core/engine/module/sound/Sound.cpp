#include "Sound.h"

#include "SDL_mixer.h"
#include "util/NumberHelper.h"

namespace MT
{

	void Sound::onInit()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		setMasterVolume(0.0);
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
		const auto soundClip = soundContainer.getSoundClip(name);

		if (soundClip != nullptr && soundClip->getChunk() != nullptr)
		{
			int vol = MT::NumberHelper::clamp<int>((int)(masterVolume * effectVolume * volume * 128.0), 0, 128);
			Mix_VolumeChunk(soundClip->getChunk(), vol);
			Mix_PlayChannel(-1, soundClip->getChunk(), 0);
		}
	}

	void Sound::stopAllSoundClips(int fadeOutMS)
	{
		Mix_FadeOutChannel(-1, fadeOutMS);
	}

	void Sound::loadMusic(std::string path, std::string name)
	{
		soundContainer.loadMusic(path, name);
	}

	void Sound::playMusic(std::string name, int fadeInMS)
	{
		if (currentMusic != name)
		{
			const auto music = soundContainer.getMusic(name);

			if (music != nullptr && music->getMusic() != nullptr)
			{
				currentMusic = name;
				Mix_FadeInMusic(music->getMusic(), -1, fadeInMS);
			}
		}
	}

	void Sound::stopMusic(std::string name, int fadeOutMS)
	{
		if (name == currentMusic || (name.empty() && !currentMusic.empty()))
		{
			Mix_FadeOutMusic(fadeOutMS);
			currentMusic.clear();
		}
	}

	void Sound::stopAllSounds(int fadeOutMS)
	{
		stopAllSoundClips(fadeOutMS);
		stopMusic("", fadeOutMS);
	}

	void Sound::setMasterVolume(double volume)
	{
		masterVolume = MT::NumberHelper::clamp(volume, 0.0, 1.0);
		setMusicVolume(musicVolume);
	}

	double Sound::getMasterVolume()
	{
		return masterVolume;
	}

	void Sound::setMusicVolume(double volume)
	{
		musicVolume = volume;
		Mix_VolumeMusic(MT::NumberHelper::clamp<int>((int)(musicVolume * masterVolume * 128.0), 0, 128));
	}

	double Sound::getMusicVolume()
	{
		return musicVolume;
	}

	void Sound::setEffectVolume(double volume)
	{
		effectVolume = volume;
	}

	double Sound::getEffectVolume()
	{
		return effectVolume;
	}

	void Sound::onCleanup()
	{
		stopAllSounds(0);
		soundContainer.cleanup();
		Mix_CloseAudio();
	}
}
