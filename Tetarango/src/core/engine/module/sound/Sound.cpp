#include "Sound.h"

namespace AW
{

	void Sound::onInit()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}

	void Sound::bindAsset(std::shared_ptr<Asset> asset)
	{
		soundContainer.bindAsset(asset);
	}

	void Sound::loadSoundClip(const std::string& path, const std::string& name)
	{
		soundContainer.loadSoundClip(path, name);
	}

	void Sound::playSoundClip(const std::string& name, double volume)
	{
		const auto soundClip = soundContainer.getSoundClip(name);

		if (soundClip != nullptr && soundClip->getChunk() != nullptr)
		{
			int vol = AW::NumberHelper::clamp<int>((int)(masterVolume * effectVolume * volume * 128.0), 0, 128);
			Mix_VolumeChunk(soundClip->getChunk(), vol);
			Mix_PlayChannel(-1, soundClip->getChunk(), 0);
		}
	}

	void Sound::stopAllSoundClips(int fadeOutMS)
	{
		Mix_FadeOutChannel(-1, fadeOutMS);
	}

	void Sound::loadMusic(const std::string& path, const std::string& name)
	{
		soundContainer.loadMusic(path, name);
	}

	void Sound::playMusic(const std::string& name, int fadeInMS)
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

	void Sound::stopMusic(const std::string& name, int fadeOutMS)
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
		masterVolume = AW::NumberHelper::clamp(volume, 0.0, 1.0);
		setMusicVolume(musicVolume);
	}

	double Sound::getMasterVolume()
	{
		return masterVolume;
	}

	void Sound::setMusicVolume(double volume)
	{
		musicVolume = volume;
		Mix_VolumeMusic(AW::NumberHelper::clamp<int>((int)(musicVolume * masterVolume * 128.0), 0, 128));
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

	void Sound::onBindLuaHooks(const std::shared_ptr<Lua>& lua)
	{
		lua->registerBoundFunction("loadSound", shared_from_this());
		lua->registerBoundFunction("playSound", shared_from_this());

		lua->registerBoundFunction("loadMusic", shared_from_this());
		lua->registerBoundFunction("playMusic", shared_from_this());
		lua->registerBoundFunction("stopMusic", shared_from_this());

		lua->registerBoundFunction("stopAllSounds", shared_from_this());

		lua->registerBoundFunction("setMasterVolume", shared_from_this());
		lua->registerBoundFunction("setMusicVolume", shared_from_this());
		lua->registerBoundFunction("setEffectVolume", shared_from_this());
	}

	void Sound::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == "playSound" && obj->args.size() == 1) playSoundClip(obj->args[0]);
		else if (func == "playMusic" && obj->args.size() == 2) playMusic(obj->args[0], std::stoi(obj->args[1]));
		else if (func == "playSound" && obj->args.size() == 2) playSoundClip(obj->args[0], std::stod(obj->args[1]));
		else if (func == "stopMusic" && obj->args.size() == 0) stopMusic();
		else if (func == "stopMusic" && obj->args.size() == 1) stopMusic(obj->args[0]);
		else if (func == "stopMusic" && obj->args.size() == 2) stopMusic(obj->args[0], std::stoi(obj->args[1]));
		else if (func == "stopAllSounds" && obj->args.size() == 0) stopAllSounds();
		else if (func == "stopAllSounds" && obj->args.size() == 1) stopAllSounds(std::stoi(obj->args[0]));
		else if (func == "setMasterVolume" && obj->args.size() == 1) setMasterVolume(std::stod(obj->args[0]));
		else if (func == "setMusicVolume" && obj->args.size() == 1) setMusicVolume(std::stod(obj->args[0]));
		else if (func == "setEffectVolume" && obj->args.size() == 1) setEffectVolume(std::stod(obj->args[0]));
		else if (func == "loadSound" && obj->args.size() == 2) loadSoundClip("res/sound/" + obj->args[0], obj->args[1]);
		else if (func == "loadMusic" && obj->args.size() == 2) loadMusic("res/sound/" + obj->args[0], obj->args[1]);
	}
}
