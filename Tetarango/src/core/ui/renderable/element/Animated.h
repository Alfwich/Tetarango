#pragma once

#include <SDL.h>
#include "Element.h"
#include "engine/module/animation/Animation.h"
#include "engine/module/animation/AnimationSet.h"
#include "engine/module/time/Timer.h"
#include "util/Rect.h"

namespace AW
{


	class Animated : public Element
	{
		std::shared_ptr<Animation> currentAnimation;
		bool playing = false, paused = false, prefixHasChanged = false;
		int frame, fpsOffset;
		std::shared_ptr<Timer> frameTimer;
		std::string animationPrefix;
		void updateFrame(double frameTime);

	protected:
		std::string currentAnimationName, defaultAnimationName, currentAnimationSetName;
		std::shared_ptr<AnimationSet> animationSet;

	public:
		bool sizeToAnimation = true;

		Animated();
		virtual void play(std::string animationName = "");
		void pause();
		void stop();
		bool isPlaying();
		bool isPaused();

		virtual void setAnimationSet(std::shared_ptr<AnimationSet> animationSet, std::string setName = "");
		void setAnimationSet(std::string animationSetName);
		void setCurrentAnimation(std::string animationName);

		std::string getCurrentAnimationName();
		void setFpsOffset(int fpsOffset);
		void setDefaultAnimationName(std::string name);
		void setAnimationPrefix(std::string prefix);

		std::shared_ptr<Rect> getTextureClipRect();
		virtual void enterFrame(double frameTime);

		virtual void onInitialAttach();
		virtual void onAttach();
		virtual void onTimeScopeChanged();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};

}
