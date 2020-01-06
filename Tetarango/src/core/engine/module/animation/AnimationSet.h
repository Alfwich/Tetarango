#pragma once

#include "Animation.h"

namespace AW
{
	class AnimationSet
	{
		std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
		bool isLoaded = false;

	public:
		void load();
		std::shared_ptr<Animation> getAnimation(std::string name);

		virtual void onLoadAnimations() { /* NO-OP */ };
		std::shared_ptr<Animation> startNewAnimation(std::string name);

		const std::unordered_map<std::string, std::shared_ptr<Animation>>& getAnimations();
	};

}
