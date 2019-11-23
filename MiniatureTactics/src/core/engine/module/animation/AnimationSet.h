#pragma once

#include <unordered_map>
#include <string>
#include <SDL.h>
#include "Animation.h"
#include "engine/module/logger/Logger.h"

namespace MT
{
	class AnimationSet
	{
		std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
		bool isLoaded = false;

	protected:
		std::shared_ptr<Animation> startNewAnimation(std::string name);
		virtual void onLoadAnimations() { /* NO-OP */ };

	public:
		void load();
		std::shared_ptr<Animation> getAnimation(std::string name);
	};

}
