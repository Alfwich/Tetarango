#include "AnimationSet.h"

namespace AW
{



	std::shared_ptr<Animation> AnimationSet::startNewAnimation(std::string name)
	{
		std::shared_ptr<Animation> newAnimation = std::make_unique<Animation>();
		animations[name] = newAnimation;
		return newAnimation;
	}

	const std::unordered_map<std::string, std::shared_ptr<Animation>>& AnimationSet::getAnimations()
	{
		return animations;
	}

	void AnimationSet::load()
	{
		if (!isLoaded)
		{
			isLoaded = true;
			onLoadAnimations();
		}
	}

	std::shared_ptr<Animation> AnimationSet::getAnimation(std::string name)
	{
		if (animations.count(name) == 0)
		{
			Logger::instance()->log("AnimationSet::Failed to find Animation for key: " + name);
			return nullptr;
		}

		return animations[name];
	}
}
