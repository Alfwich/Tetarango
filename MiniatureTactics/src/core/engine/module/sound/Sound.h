#pragma once

#include "SDL.h"
#include "SDL_mixer.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"
#include "SoundContainer.h"
#include "SoundClip.h"

namespace MT
{

	class Sound : public IBaseModule
	{
		SoundContainer soundContainer;

	public:
		void onInit();

		void bindAsset(std::shared_ptr<Asset> asset);

		void loadSoundClip(std::string path, std::string name);
		void playSoundClip(std::string name, double volume);
	};

}


