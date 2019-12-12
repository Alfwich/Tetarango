#include "SceneOptionsMenu.h"
#include "gui/camera/GameCamera.h"

namespace
{
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

	void SceneOptionsMenu::onAttach()
	{
		BaseScene::onAttach();
		config = modules->screen->getCurrentScreenConfig();
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

		applyButton = std::make_shared<ButtonBasic>();
		applyButton->setText("Apply");
		applyButton->clickListener = weak_from_this();
		applyButton->setPosition(getScreenWidth() / 2.0, getScreenHeight() - 100.0);
		applyButton->setEnabled(false);
		add(applyButton);

		resetButton = std::make_shared<ButtonBasic>();
		resetButton->setText("Reset");
		resetButton->clickListener = weak_from_this();
		resetButton->toRightOf(applyButton, 5.0, 0.0);
		add(resetButton);

		backButton = std::make_shared<ButtonBasic>();
		backButton->setText("Back");
		backButton->clickListener = weak_from_this();
		backButton->toLeftOf(applyButton, 5.0, 0.0);
		add(backButton);

		scrollArea = std::make_shared<ScrollArea>();
		scrollArea->setScrollerEnabled(false);
		add(scrollArea);

		resolutionButtons.clear();
		std::shared_ptr<ButtonBasic> prevResolutionButton;
		for (const auto resolution : info.resolutions)
		{
			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			if (config.mode == MT::ScreenModes::FullscreenDesktop || resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
			{
				resolutionButton->setEnabled(false);
			}

			resolutionButton->clickListener = weak_from_this();
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
			scrollArea->add(resolutionButton);
		}

		scrollArea->centerAlignSelf(5.0, 0.0);

		const auto checkboxYOffset = 10.0;
		const auto checkboxYGroupOffset = 20.0;
		fullscreenCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenCheckbox->setText("Fullscreen");
		fullscreenCheckbox->toRightOf(scrollArea, 19.0, -scrollArea->getHalfHeight() + fullscreenCheckbox->getHalfHeight() + 5.0);
		fullscreenCheckbox->setChecked(config.mode == MT::ScreenModes::Fullscreen);
		fullscreenCheckbox->clickListener = weak_from_this();
		add(fullscreenCheckbox);

		fullscreenDesktopCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenDesktopCheckbox->setText("Fullscreen Windowed");
		fullscreenDesktopCheckbox->toBottomOf(fullscreenCheckbox, 0.0, checkboxYOffset);
		fullscreenDesktopCheckbox->setChecked(config.mode == MT::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->clickListener = weak_from_this();
		add(fullscreenDesktopCheckbox);

		windowedCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		windowedCheckbox->setText("Window");
		windowedCheckbox->toBottomOf(fullscreenDesktopCheckbox, 0.0, checkboxYOffset);
		windowedCheckbox->setChecked(config.mode == MT::ScreenModes::Windowed);
		windowedCheckbox->clickListener = weak_from_this();
		add(windowedCheckbox);

		msaaOffCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaaOffCheckbox->setText("MSAA Off");
		msaaOffCheckbox->toBottomOf(windowedCheckbox, 0.0, checkboxYOffset + checkboxYGroupOffset);
		msaaOffCheckbox->setChecked(config.msaaSamples == 0);
		msaaOffCheckbox->clickListener = weak_from_this();
		add(msaaOffCheckbox);

		msaa2xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa2xCheckbox->setText("MSAA 2x");
		msaa2xCheckbox->toBottomOf(msaaOffCheckbox, 0.0, checkboxYOffset);
		msaa2xCheckbox->setChecked(config.msaaSamples == 2);
		msaa2xCheckbox->clickListener = weak_from_this();
		add(msaa2xCheckbox);

		msaa4xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa4xCheckbox->setText("MSAA 4x");
		msaa4xCheckbox->toBottomOf(msaa2xCheckbox, 0.0, checkboxYOffset);
		msaa4xCheckbox->setChecked(config.msaaSamples == 4);
		msaa4xCheckbox->clickListener = weak_from_this();
		add(msaa4xCheckbox);

		msaa8xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa8xCheckbox->setText("MSAA 8x");
		msaa8xCheckbox->toBottomOf(msaa4xCheckbox, 0.0, checkboxYOffset);
		msaa8xCheckbox->setChecked(config.msaaSamples == 8);
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

		auto shouldEnableApply = false;
		auto shouldNotifyApplication = false;
		if (id == resetButton->getId())
		{
			config = MT::ScreenConfig();
			shouldNotifyApplication = true;
		}

		if (id == applyButton->getId())
		{
			shouldNotifyApplication = true;
		}

		if (id == fullscreenCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(MT::ScreenModes::Fullscreen);
		}

		if (id == windowedCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(MT::ScreenModes::Windowed);
		}

		if (id == fullscreenDesktopCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(MT::ScreenModes::FullscreenDesktop);
		}

		if (id == msaaOffCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(0);
		}

		if (id == msaa2xCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(2);
		}

		if (id == msaa4xCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(4);
		}

		if (id == msaa8xCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(8);
		}

		if (id == openGlCompatibilityModeCheckbox->getId())
		{
			config.openGLCompatibilityMode = !config.openGLCompatibilityMode;
			openGlCompatibilityModeCheckbox->setChecked(config.openGLCompatibilityMode);
			shouldEnableApply = true;
		}

		if (id == wireframeModeCheckbox->getId())
		{
			config.openGlWireframeMode = !config.openGlWireframeMode;
			wireframeModeCheckbox->setChecked(config.openGlWireframeMode);
			shouldEnableApply = true;
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (resolutionButton->getId() == id)
			{
				auto newResolution = resolutionButton->getText();
				config.width = MT::StringHelper::getDisplayComponentForDisplayString(&newResolution, 0);
				config.height = MT::StringHelper::getDisplayComponentForDisplayString(&newResolution, 1);
				shouldEnableApply = true;
			}
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (config.mode == MT::ScreenModes::FullscreenDesktop || (resolutionButton->getText() == std::to_string(config.width) + "x" + std::to_string(config.height)))
			{
				resolutionButton->setEnabled(false);
			}
			else
			{
				resolutionButton->setEnabled(true);
			}
		}

		if (shouldEnableApply)
		{
			applyButton->setEnabled(true);
		}

		if (shouldNotifyApplication)
		{
			modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
			applyButton->setEnabled(false);
		}
	}

	bool SceneOptionsMenu::setMsaaMode(int samples)
	{
		if (config.msaaSamples == samples)
		{
			return false;
		}

		msaaOffCheckbox->setChecked(false);
		msaa2xCheckbox->setChecked(false);
		msaa4xCheckbox->setChecked(false);
		msaa8xCheckbox->setChecked(false);

		switch (samples)
		{
		case 2:
			msaa2xCheckbox->setChecked(true);
			break;

		case 4:
			msaa4xCheckbox->setChecked(true);
			break;

		case 8:
			msaa8xCheckbox->setChecked(true);
			break;

		default:
		case 0:
			msaaOffCheckbox->setChecked(true);
			samples = 0;
			break;
		}

		config.msaaSamples = samples;

		return true;
	}

	bool SceneOptionsMenu::setScreenMode(MT::ScreenModes mode)
	{
		if (config.mode == mode)
		{
			return false;
		}

		fullscreenCheckbox->setChecked(false);
		windowedCheckbox->setChecked(false);
		fullscreenDesktopCheckbox->setChecked(false);

		switch (mode)
		{
		case MT::ScreenModes::Fullscreen:
			fullscreenCheckbox->setChecked(true);
			break;

		case MT::ScreenModes::Windowed:
			windowedCheckbox->setChecked(true);
			break;

		case MT::ScreenModes::FullscreenDesktop:
			fullscreenDesktopCheckbox->setChecked(true);
			break;
		}

		config.mode = mode;

		return true;
	}


}