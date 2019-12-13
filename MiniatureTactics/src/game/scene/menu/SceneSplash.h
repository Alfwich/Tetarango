#pragma once

#include "engine/module/animation/Transition.h"
#include "ui/renderable/element/Text.h"
#include "engine/module/time/Timer.h"
#include "scene/BaseScene.h"

namespace MTGame
{

	class SceneSplash : public BaseScene
	{
		std::shared_ptr<MT::Transition> splashTransition;
		std::shared_ptr<MT::Timer> transitionTimer;
		int state = 0;

		std::shared_ptr<MT::Text> splashText;
		std::shared_ptr<MT::Element> splashImage;

	public:
		SceneSplash();

		void onLoadResources();
		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onAttach();
		void onEnterFrame(double frameTime);

		void onMouseButtonLeftDown();
		void onKeyPressed(SDL_Scancode key);
	};

}
