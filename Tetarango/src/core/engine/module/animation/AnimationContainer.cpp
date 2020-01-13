#include "AnimationContainer.h"

namespace AW
{
	void AnimationContainer::bindTime(std::shared_ptr<Time> time)
	{
		this->time = time;
	}

	void AnimationContainer::bindEvent(std::shared_ptr<Event> event)
	{
		this->event = event;
	}

	void AnimationContainer::addAnimationSet(std::shared_ptr<AnimationSet> animationSet, std::string name)
	{
		animationSet->load();
		animationSets[name] = animationSet;
		Logger::instance()->log("AnimationContainer::Added animation set name=" + name);
	}

	std::shared_ptr<AnimationSet> AnimationContainer::getAnimationSet(std::string name)
	{
		if (animationSets.count(name) == 0)
		{
			Logger::instance()->logCritical("AnimationContainer::Failed to find animation set for key= " + name);
			return nullptr;
		}

		return animationSets[name];
	}

	bool AnimationContainer::hasAnimationSet(std::string name)
	{
		return animationSets.count(name) == 1;
	}

	std::shared_ptr<Transition> AnimationContainer::createTransition()
	{
		return createTransitionForTimeScope(TimeScope::Global);
	}

	std::shared_ptr<Transition> AnimationContainer::createGameTransition()
	{
		return createTransitionForTimeScope(TimeScope::Game);
	}

	std::shared_ptr<Transition> AnimationContainer::createTransitionForTimeScope(TimeScope scopeName)
	{
		auto transition = std::make_shared<Transition>(time, scopeName);

		transition->enterFrameActivated = true;
		event->registerOnEnterFrame(std::static_pointer_cast<EnterFrameListener>(transition));

		return transition;
	}
}
