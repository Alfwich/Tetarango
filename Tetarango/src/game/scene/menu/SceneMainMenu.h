#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"
#include "scene/BaseScene.h"
#include "gui/button/ButtonBasic.h"
#include "prop/title/TitleGame.h"

namespace AWGame
{

	class SceneMainMenu : public BaseScene
	{
		int state = 0;

		std::shared_ptr<TitleGame> gameTitle;
		std::shared_ptr<AWCore::Transition> gameTitleTransition;

		std::shared_ptr<AWCore::Container> rootContainer;
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