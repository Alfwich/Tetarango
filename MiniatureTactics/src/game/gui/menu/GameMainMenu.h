#pragma once

#include "ui/renderable/Container.h"
#include "gui/button/ButtonBasic.h"
#include "gui/BaseGui.h"

namespace MTGame
{

	class GameMainMenu : public BaseGui, public MT::Container, public IButtonListener
	{
		std::shared_ptr<ButtonBasic> backButton;
		std::shared_ptr<ButtonBasic> optionsButton;
		std::shared_ptr<ButtonBasic> saveButton;
		std::shared_ptr<ButtonBasic> mainMenuButton;

	public:
		GameMainMenu();

		void onInitialAttach();
		void onCreateChildren();

		void onButtonClicked(int id);

		void show();
		void hide();
	};

}
