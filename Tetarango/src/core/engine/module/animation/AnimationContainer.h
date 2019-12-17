#pragma once

#include <unordered_map>
#include <string>
#include "AnimationSet.h"
#include "Transition.h"
#include "engine/module/logger/Logger.h"
#include "engine/module/event/Event.h"
#include "engine/module/IBaseModule.h"

namespace AW
{
	class AnimationContainer : public IBaseModule
	{
		std::shared_ptr<Time> time;
		std::shared_ptr<Event> event;
		std::unordered_map<std::string, std::shared_ptr<AnimationSet>> animationSets;

	public:

		void bindTime(std::shared_ptr<Time> time);
		void bindEvent(std::shared_ptr<Event> event);

		void addAnimationSet(std::shared_ptr<AnimationSet> animationSet, std::string name);
		std::shared_ptr<AnimationSet> getAnimationSet(std::string name);
		bool hasAnimationSet(std::string name);

		std::shared_ptr<Transition> createTransition();
		std::shared_ptr<Transition> createGameTransition();
		std::shared_ptr<Transition> createTransitionForTimeScope(TimeScope scopeName);
	};
}
