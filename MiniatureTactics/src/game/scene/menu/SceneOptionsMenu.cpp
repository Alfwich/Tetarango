#include "SceneOptionsMenu.h"

namespace
{
	const char* backButtonId = "back_button";
}


namespace MTGame
{


	SceneOptionsMenu::SceneOptionsMenu() : BaseScene(SceneGame::OptionsMenu)
	{
		enableSerialization<SceneOptionsMenu>();
	}

	void SceneOptionsMenu::onCreateChildren()
	{
		optionsMenuTitle = std::make_shared<MT::Text>();
		optionsMenuTitle->setFont("medium", 28);
		optionsMenuTitle->setText("Options");
		optionsMenuTitle->setPosition(getScreenWidth() / 2.0, 50.0);
		add(optionsMenuTitle);

		backButton = std::make_shared<ButtonBasic>();
		backButton->setText("Back");
		backButton->name = backButtonId;
		backButton->setPosition(getScreenWidth() / 2.0, getScreenHeight() - 100.0);
		backButton->clickListener = weak_from_this();
		add(backButton);
	}

	void SceneOptionsMenu::onChildrenHydrated()
	{
		backButton = findChildWithName<ButtonBasic>(backButtonId);
		backButton->clickListener = baseSceneWeakThisRef();
	}

	void SceneOptionsMenu::onButtonClicked(int id)
	{
		if (id == backButton->getId())
		{
			transitionToScene(SceneGame::MainMenu);
		}
	}

}