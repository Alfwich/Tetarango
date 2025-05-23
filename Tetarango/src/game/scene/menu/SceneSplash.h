#pragma once

#include "engine/module/animation/Transition.h"
#include "ui/renderable/element/Text.h"
#include "engine/module/time/Timer.h"
#include "scene/BaseScene.h"
#include "prop/block/Block.h"
#include "ui/renderable/physic/Box.h"
#include "prop/title/TitleGame.h"

namespace AWGame
{

	class SceneSplash : public BaseScene, public AW::INotifyOnTransition
	{
		std::shared_ptr<AW::Transition> splashTransition;
		int state = 0;
		double animationOffset = 0.0;

		std::shared_ptr<AW::Text> splashText;
		std::shared_ptr<AW::Element> splashImage;
		std::shared_ptr<TitleGame> titleGame;

		std::shared_ptr<AW::Box> titleGameCollider, bottomCollider;

		std::shared_ptr<AW::Element> loadingProgressBar;

		bool tryToGotoNextState(double position, double timeThreshold);

	public:
		SceneSplash();

		void onLoadResources();

		void onCreateChildren();
		void onInitialAttach();
		void onAttach();

		void onTransitionAnimationFrame(double position);
		void onTransitionCompleted();

		void onMouseButtonLeftDown();
		void onKeyPressed(AWKey key);
	};

}
