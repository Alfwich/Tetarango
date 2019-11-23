#pragma once

#pragma once

#include "engine/module/animation/AnimationSet.h"

namespace MTGame
{

	class ButtonBasicAnimation : public MT::AnimationSet
	{
	public:
		void onLoadAnimations()
		{
			int fps = 15;
			MT::RectI frameSize = {
				0,
				0,
				350,
				150
			};

			{
				auto anim = startNewAnimation("default");
				anim->setFps(fps);
				anim->addGeneralFrames(0, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = startNewAnimation("pressed");
				anim->setFps(fps);
				anim->addGeneralFrames(frameSize.w * 1, 0, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = startNewAnimation("hover");
				anim->setFps(fps);
				anim->addGeneralFrames(0, frameSize.h, frameSize.w, frameSize.h, 1);
			}
		}
	};

}
