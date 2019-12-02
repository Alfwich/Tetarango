#include "SceneOptionsMenu.h"

namespace
{
	const auto backButtonId = "back_button";
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
		modules->input->mouse->registerMouseWheel(weak_from_this());
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
		backButton->clickListener = weak_from_this();
		backButton->setPosition(getScreenWidth() / 2.0, getScreenHeight() - 100.0);
		add(backButton);

		scrollContainer = std::make_shared<MT::ScrollContainer>();
		scrollContainer->setMouseWheenEnabled(true);
		scrollContainer->setScrollAmountInPixels(43);
		add(scrollContainer);

		resolutionButtons.clear();
		std::shared_ptr<ButtonBasic> prevResolutionButton;
		for (const auto resolution : info.resolutions)
		{
			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			if (resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
			{
				resolutionButton->setColor(0x333333ff);
			}
			else
			{
				resolutionButton->clickListener = weak_from_this();
			}
			resolutionButtons.push_back(resolutionButton);

			if (prevResolutionButton != nullptr)
			{
				resolutionButton->toBottomOf(prevResolutionButton, 0, 5);
			}
			else
			{
				resolutionButton->centerAlignSelf();
			}

			prevResolutionButton = resolutionButton;
			scrollContainer->add(resolutionButton);
		}

		scrollContainer->centerAlignSelf(5.0, 0.0);
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
}