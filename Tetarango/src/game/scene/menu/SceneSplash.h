#pragma once

#include "engine/module/animation/Transition.h"
#include "ui/renderable/element/Text.h"
#include "engine/module/time/Timer.h"
#include "scene/BaseScene.h"
#include "prop/block/Block.h"
#include "prop/title/TitleGame.h"

namespace AWGame
{

	class SceneSplash : public BaseScene, public AWCore::INotifyOnTransition
	{
		std::shared_ptr<AWCore::Transition> splashTransition;
		int state = 0;
		double animationOffset = 0.0;

		std::shared_ptr<AWCore::Container> blockContainer;
		std::shared_ptr<AWCore::Text> splashText;
		std::shared_ptr<AWCore::Element> splashImage;
		std::vector<std::shared_ptr<Block>> blocks;
		std::shared_ptr<TitleGame> titleGame;

		std::shared_ptr<AWCore::Element> loadingProgressBar;

		bool tryToGotoNextState(double position, double timeThreshold);

	public:
		SceneSplash();

		void onLoadResources();
		void onCreateChildren();
		void onInitialAttach();
		void onAttach();

		void onEnterFrame(double frameTime);

		void onTransitionAnimationFrame(double position);
		void onTransitionCompleted();

		void onMouseButtonLeftDown();
		void onKeyPressed(SDL_Scancode key);
	};

}
