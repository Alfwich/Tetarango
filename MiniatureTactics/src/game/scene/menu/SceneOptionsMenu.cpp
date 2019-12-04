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

		scrollContainer = std::make_shared<MT::ScrollContainer>();
		scrollContainer->setMouseWheenEnabled(config.mode != MT::ScreenModes::FullscreenDesktop);
		scrollContainer->setScrollAmountInPixels(43);
		add(scrollContainer);

		resolutionButtons.clear();
		std::shared_ptr<ButtonBasic> prevResolutionButton;
		for (const auto resolution : info.resolutions)
		{
			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			if (config.mode == MT::ScreenModes::FullscreenDesktop || resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
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

		const auto checkboxYOffset = 10.0;
		const auto checkboxYGroupOffset = 20.0;
		fullscreenCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenCheckbox->setText("Fullscreen");
		fullscreenCheckbox->toRightOf(scrollContainer, 5.0, -scrollContainer->getHalfHeight() + fullscreenCheckbox->getHalfHeight() + 5.0);
		fullscreenCheckbox->setChecked(config.mode == MT::ScreenModes::Fullscreen);
		fullscreenCheckbox->setEnabled(config.mode == MT::ScreenModes::Fullscreen);
		fullscreenCheckbox->clickListener = weak_from_this();
		add(fullscreenCheckbox);

		fullscreenDesktopCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenDesktopCheckbox->setText("Fullscreen Windowed");
		fullscreenDesktopCheckbox->toBottomOf(fullscreenCheckbox, 0.0, checkboxYOffset);
		fullscreenDesktopCheckbox->setChecked(config.mode == MT::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->setEnabled(config.mode == MT::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->clickListener = weak_from_this();
		add(fullscreenDesktopCheckbox);

		windowedCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		windowedCheckbox->setText("Window");
		windowedCheckbox->toBottomOf(fullscreenDesktopCheckbox, 0.0, checkboxYOffset);
		windowedCheckbox->setChecked(config.mode == MT::ScreenModes::Windowed);
		windowedCheckbox->setEnabled(config.mode == MT::ScreenModes::Windowed);
		windowedCheckbox->clickListener = weak_from_this();
		add(windowedCheckbox);

		msaaOffCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaaOffCheckbox->setText("MSAA Off");
		msaaOffCheckbox->toBottomOf(windowedCheckbox, 0.0, checkboxYOffset + checkboxYGroupOffset);
		msaaOffCheckbox->setChecked(config.msaaSamples == 0);
		msaaOffCheckbox->setEnabled(config.msaaSamples == 0);
		msaaOffCheckbox->clickListener = weak_from_this();
		add(msaaOffCheckbox);

		msaa2xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa2xCheckbox->setText("MSAA 2x");
		msaa2xCheckbox->toBottomOf(msaaOffCheckbox, 0.0, checkboxYOffset);
		msaa2xCheckbox->setChecked(config.msaaSamples == 2);
		msaa2xCheckbox->setEnabled(config.msaaSamples == 2);
		msaa2xCheckbox->clickListener = weak_from_this();
		add(msaa2xCheckbox);

		msaa4xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa4xCheckbox->setText("MSAA 4x");
		msaa4xCheckbox->toBottomOf(msaa2xCheckbox, 0.0, checkboxYOffset);
		msaa4xCheckbox->setChecked(config.msaaSamples == 4);
		msaa4xCheckbox->setEnabled(config.msaaSamples == 4);
		msaa4xCheckbox->clickListener = weak_from_this();
		add(msaa4xCheckbox);

		msaa8xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa8xCheckbox->setText("MSAA 8x");
		msaa8xCheckbox->toBottomOf(msaa4xCheckbox, 0.0, checkboxYOffset);
		msaa8xCheckbox->setChecked(config.msaaSamples == 8);
		msaa8xCheckbox->setEnabled(config.msaaSamples == 8);
		msaa8xCheckbox->clickListener = weak_from_this();
		add(msaa8xCheckbox);

		openGlCompatibilityModeCheckbox = std::make_shared<CheckBoxBasic>();
		openGlCompatibilityModeCheckbox->setText("OpenGL Compatibility Mode");
		openGlCompatibilityModeCheckbox->toBottomOf(msaa8xCheckbox, 0.0, checkboxYOffset + checkboxYGroupOffset);
		openGlCompatibilityModeCheckbox->setChecked(config.openGLCompatibilityMode);
		openGlCompatibilityModeCheckbox->clickListener = weak_from_this();
		add(openGlCompatibilityModeCheckbox);

		wireframeModeCheckbox = std::make_shared<CheckBoxBasic>();
		wireframeModeCheckbox->setText("Wireframe Mode");
		wireframeModeCheckbox->toBottomOf(openGlCompatibilityModeCheckbox, 0.0, checkboxYOffset);
		wireframeModeCheckbox->setChecked(config.openGlWireframeMode);
		wireframeModeCheckbox->clickListener = weak_from_this();
		add(wireframeModeCheckbox);

	}

	void SceneOptionsMenu::onButtonClicked(int id)
	{
		if (id == backButton->getId())
		{
			transitionToScene(SceneGame::MainMenu);
			return;
		}

		auto shouldNotifyApplication = false;
		if (id == fullscreenCheckbox->getId())
		{
			config.mode = MT::ScreenModes::Fullscreen;
			shouldNotifyApplication = true;
		}

		if (id == windowedCheckbox->getId())
		{
			config.mode = MT::ScreenModes::Windowed;
			shouldNotifyApplication = true;
		}

		if (id == fullscreenDesktopCheckbox->getId())
		{
			config.mode = MT::ScreenModes::FullscreenDesktop;
			shouldNotifyApplication = true;
		}

		if (id == msaaOffCheckbox->getId())
		{
			config.msaaSamples = 0;
			shouldNotifyApplication = true;
		}

		if (id == msaa2xCheckbox->getId())
		{
			config.msaaSamples = 2;
			shouldNotifyApplication = true;
		}

		if (id == msaa4xCheckbox->getId())
		{
			config.msaaSamples = 4;
			shouldNotifyApplication = true;
		}

		if (id == msaa8xCheckbox->getId())
		{
			config.msaaSamples = 8;
			shouldNotifyApplication = true;
		}

		if (id == openGlCompatibilityModeCheckbox->getId())
		{
			config.openGLCompatibilityMode = !config.openGLCompatibilityMode;
			shouldNotifyApplication = true;
		}

		if (id == wireframeModeCheckbox->getId())
		{
			config.openGlWireframeMode = !config.openGlWireframeMode;
			shouldNotifyApplication = true;
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (shouldNotifyApplication)
			{
				break;
			}

			if (resolutionButton->getId() == id)
			{
				auto newResolution = resolutionButton->getText();
				config.width = MT::StringHelper::getDisplayComponentForDisplayString(&newResolution, 0);
				config.height = MT::StringHelper::getDisplayComponentForDisplayString(&newResolution, 1);
				shouldNotifyApplication = true;
			}
		}

		if (shouldNotifyApplication)
		{
			modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
		}
	}
}