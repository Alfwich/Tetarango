#pragma once

#include <SDL.h>
#include "Element.h"
#include "engine/module/animation/Animation.h"
#include "engine/module/animation/AnimationSet.h"
#include "engine/module/time/Timer.h"
#include "util/Rect.h"

namespace MT
{


	class Animated : public Element
	{
		std::shared_ptr<AnimationSet> animationSet;
		std::shared_ptr<Animation> currentAnimation;
		bool playing = false, paused = false;
		int frame, fpsOffset;
		std::shared_ptr<Timer> frameTimer;
		void updateFrame(double frameTime);

	protected:
		std::string currentAnimationName, defaultAnimationName, currentAnimationSetName;

	public:
		bool sizeToAnimation = true;

		Animated();
		void play(std::string animationName = "");
		void pause();
		void stop();
		bool isPlaying();
		bool isPaused();

		void setAnimationSet(std::shared_ptr<AnimationSet> animationSet);
		void setAnimationSet(std::string animationSetName);
		void setCurrentAnimation(std::string animationName);
		std::string getCurrentAnimationName();
		void setFpsOffset(int fpsOffset);
		void setDefaultAnimationName(std::string name);

		std::shared_ptr<Rect> getClipRect();
		virtual void enterFrame(double frameTime);

		virtual void onInitialAttach();
		virtual void onAttach();
		virtual void onTimeScopeChanged();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};

}
