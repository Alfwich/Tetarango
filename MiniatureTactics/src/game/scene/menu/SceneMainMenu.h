#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"
#include "scene/BaseScene.h"
#include "gui/button/ButtonBasic.h"

namespace MTGame
{

	class SceneMainMenu : public BaseScene
	{
		int state = 0;

		std::shared_ptr<MT::Text> gameTitleText;
		std::shared_ptr<MT::Transition> gameTitleTransition;

		std::shared_ptr<MT::Container> rootContainer;
		std::shared_ptr<ButtonBasic> playButton;
		std::shared_ptr<ButtonBasic> optionButton;
		std::shared_ptr<ButtonBasic> exitButton;

	public:
		SceneMainMenu();

		void onInitialAttach();
		void onAttach();
		void onCreateChildren();

		void onButtonClicked(int id);
	};

}