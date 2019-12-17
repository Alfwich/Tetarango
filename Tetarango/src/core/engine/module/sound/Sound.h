#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"
#include "SoundContainer.h"
#include "SoundClip.h"

namespace AW
{

	class Sound : public IBaseModule
	{
		SoundContainer soundContainer;
		std::string currentMusic;

		double masterVolume = 0.0, musicVolume = 0.0, effectVolume = 0.0;

	public:
		void onInit();

		void bindAsset(std::shared_ptr<Asset> asset);

		void loadSoundClip(std::string path, std::string name);
		void playSoundClip(std::string name, double volume = 1.0);
		void stopAllSoundClips(int fadeOutMS = 1000);

		void loadMusic(std::string path, std::string name);
		void playMusic(std::string name, int fadeInMS = 1000);
		void stopMusic(std::string name = "", int fadeOutMS = 1000);

		void stopAllSounds(int fadeOutMS = 1000);

		void setMasterVolume(double volume);
		double getMasterVolume();

		void setMusicVolume(double volume);
		double getMusicVolume();

		void setEffectVolume(double volume);
		double getEffectVolume();

		void onCleanup();
	};

}


