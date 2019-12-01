#include "SceneOptionsMenu.h"

namespace
{
	const char* backButtonId = "back_button";
}


namespace MTGame
{


	SceneOptionsMenu::SceneOptionsMenu() : BaseScene(SceneGame::OptionsMenu)
	{
		rebuildOnLoad = true;
		enableSerialization<SceneOptionsMenu>();
	}

	void SceneOptionsMenu::onInitialAttach()
	{
		BaseScene::onInitialAttach();

		info = modules->screen->getAllSupportedDisplayModes();
	}

	void SceneOptionsMenu::onDestroyChildren()
	{
		resolutionButtons.clear();
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

		std::shared_ptr<ButtonBasic> prevResolutionButton;
		for (const auto resolution : info.resolutions)
		{
			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			resolutionButton->clickListener = weak_from_this();

			if (prevResolutionButton != nullptr)
			{
				resolutionButton->toBottomOf(prevResolutionButton, 2);
			}
			else
			{
				resolutionButton->setPosition(0 + resolutionButton->getHalfWidth() + 2, 0 + resolutionButton->getHalfHeight() + 2);
			}

			resolutionButtons.push_back(resolutionButton);
			add(resolutionButton);

			prevResolutionButton = resolutionButton;
		}
	}

	void SceneOptionsMenu::onButtonClicked(int id)
	{
		if (id == backButton->getId())
		{
			transitionToScene(SceneGame::MainMenu);
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (resolutionButton->getId() == id)
			{
				modules->event->pushEvent(std::make_shared<MT::ApplicationEvent>(resolutionButton->getText(), MT::Events::CHANGE_RESOLUTION));
			}
		}
	}

	void SceneOptionsMenu::onDisplayProvisioned()
	{
		rebuild();
	}
}