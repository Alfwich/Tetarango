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

		void loadSoundClip(const std::string& path, const std::string& name);
		void playSoundClip(const std::string& name, double volume = 1.0);
		void stopAllSoundClips(int fadeOutMS = 1000);

		void loadMusic(const std::string& path, const std::string& name);
		void playMusic(const std::string& name, int fadeInMS = 1000);
		void stopMusic(const std::string& name = "", int fadeOutMS = 1000);

		void stopAllSounds(int fadeOutMS = 1000);

		void setMasterVolume(double volume);
		double getMasterVolume();

		void setMusicVolume(double volume);
		double getMusicVolume();

		void setEffectVolume(double volume);
		double getEffectVolume();

		void onCleanup();

		virtual void onBindLuaHooks(const std::shared_ptr<Lua>& lua) override;

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override { return "sound"; };
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};

}


