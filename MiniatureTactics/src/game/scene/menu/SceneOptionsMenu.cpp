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
		config = modules->screen->getCurrentScreenConfig();
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

		fullscreenCheckbox = std::make_shared<CheckBoxBasic>();
		fullscreenCheckbox->setText("Fullscreen");
		fullscreenCheckbox->setPosition(getScreenWidth() / 2.0, getScreenHeight() / 2.0 - 100.0);
		fullscreenCheckbox->setChecked(config.mode == MT::ScreenModes::Fullscreen);
		fullscreenCheckbox->setEnabled(config.mode == MT::ScreenModes::Fullscreen);
		fullscreenCheckbox->clickListener = weak_from_this();
		add(fullscreenCheckbox);

		fullscreenDesktopCheckbox = std::make_shared<CheckBoxBasic>();
		fullscreenDesktopCheckbox->setText("Fullscreen Windowed");
		fullscreenDesktopCheckbox->toBottomOf(fullscreenCheckbox, 0.0, 50.0);
		fullscreenDesktopCheckbox->setChecked(config.mode == MT::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->setEnabled(config.mode == MT::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->clickListener = weak_from_this();
		add(fullscreenDesktopCheckbox);

		windowedCheckbox = std::make_shared<CheckBoxBasic>();
		windowedCheckbox->setText("Window");
		windowedCheckbox->toBottomOf(fullscreenDesktopCheckbox, 0.0, 50.0);
		windowedCheckbox->setChecked(config.mode == MT::ScreenModes::Windowed);
		windowedCheckbox->setEnabled(config.mode == MT::ScreenModes::Windowed);
		windowedCheckbox->clickListener = weak_from_this();
		add(windowedCheckbox);

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
				resolutionButton->setEnabled(false);
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
			return;
		}

		if (id == fullscreenCheckbox->getId())
		{
			config.mode = MT::ScreenModes::Fullscreen;
			modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
			return;
		}

		if (id == windowedCheckbox->getId())
		{
			config.mode = MT::ScreenModes::Windowed;
			modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
			return;
		}

		if (id == fullscreenDesktopCheckbox->getId())
		{
			config.mode = MT::ScreenModes::FullscreenDesktop;
			modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
			return;
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (resolutionButton->getId() == id)
			{
				auto newResolution = resolutionButton->getText();
				config.width = MT::StringHelper::getDisplayComponentForDisplayString(&newResolution, 0);
				config.height = MT::StringHelper::getDisplayComponentForDisplayString(&newResolution, 1);
				modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
			}
		}
	}
}