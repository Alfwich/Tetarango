#include "SceneOptionsMenu.h"
#include "gui/camera/GameCamera.h"
#include "scene/game/SceneMainGame.h"

namespace MTGame
{
	SceneOptionsMenu::SceneOptionsMenu() : BaseScene(SceneGame::OptionsMenu)
	{
		rebuildOnLoad = true;
		enableSerialization<SceneOptionsMenu>();
	}

	void SceneOptionsMenu::onInitialAttach()
	{
		info = modules->screen->getAllSupportedDisplayModes();

		modules->input->mouse->registerMouseWheel(weak_from_this());
		modules->input->keyboard->registerKey(SDL_SCANCODE_RETURN, weak_from_this());

		config = modules->screen->getCurrentScreenConfig();
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
		add(optionsMenuTitle);

		applyButton = std::make_shared<ButtonBasic>();
		applyButton->setText("Apply");
		applyButton->clickListener = weak_from_this();
		applyButton->setEnabled(false);
		add(applyButton);

		resetButton = std::make_shared<ButtonBasic>();
		resetButton->setText("Reset");
		resetButton->clickListener = weak_from_this();
		add(resetButton);

		backButton = std::make_shared<ButtonBasic>();
		backButton->setText("Back");
		backButton->clickListener = weak_from_this();
		add(backButton);

		resolutionScrollArea = std::make_shared<ScrollArea>();
		resolutionScrollArea->setExpandToChildren(false);
		resolutionScrollArea->setSize(180.0, 505.0);
		resolutionScrollArea->setScrollerHeight(505.0);
		resolutionScrollArea->setScrollAmount(45.0);
		add(resolutionScrollArea);

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
			resolutionScrollArea->add(resolutionButton);
		}

		fullscreenCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenCheckbox->setText("Fullscreen");
		fullscreenCheckbox->setChecked(config.mode == MT::ScreenModes::Fullscreen);
		fullscreenCheckbox->clickListener = weak_from_this();
		add(fullscreenCheckbox);

		fullscreenDesktopCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenDesktopCheckbox->setText("Fullscreen Windowed");
		fullscreenDesktopCheckbox->setChecked(config.mode == MT::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->clickListener = weak_from_this();
		add(fullscreenDesktopCheckbox);

		windowedCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		windowedCheckbox->setText("Window");
		windowedCheckbox->setChecked(config.mode == MT::ScreenModes::Windowed);
		windowedCheckbox->clickListener = weak_from_this();
		add(windowedCheckbox);

		msaaOffCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaaOffCheckbox->setText("MSAA Off");
		msaaOffCheckbox->setChecked(config.msaaSamples == 0);
		msaaOffCheckbox->clickListener = weak_from_this();
		add(msaaOffCheckbox);

		msaa4xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa4xCheckbox->setText("MSAA 4x");
		msaa4xCheckbox->setChecked(config.msaaSamples == 4);
		msaa4xCheckbox->clickListener = weak_from_this();
		add(msaa4xCheckbox);

		msaa8xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa8xCheckbox->setText("MSAA 8x");
		msaa8xCheckbox->setChecked(config.msaaSamples == 8);
		msaa8xCheckbox->clickListener = weak_from_this();
		add(msaa8xCheckbox);

		msaa16xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa16xCheckbox->setText("MSAA 16x");
		msaa16xCheckbox->setChecked(config.msaaSamples == 16);
		msaa16xCheckbox->clickListener = weak_from_this();
		add(msaa16xCheckbox);

		openGlCompatibilityModeCheckbox = std::make_shared<CheckBoxBasic>();
		openGlCompatibilityModeCheckbox->setText("OpenGL Compatibility Mode");
		openGlCompatibilityModeCheckbox->setChecked(config.openGLCompatibilityMode);
		openGlCompatibilityModeCheckbox->clickListener = weak_from_this();
		add(openGlCompatibilityModeCheckbox);

		wireframeModeCheckbox = std::make_shared<CheckBoxBasic>();
		wireframeModeCheckbox->setText("Wireframe Mode");
		wireframeModeCheckbox->setChecked(config.openGlWireframeMode);
		wireframeModeCheckbox->clickListener = weak_from_this();
		add(wireframeModeCheckbox);
	}

	void SceneOptionsMenu::onLayoutChildren()
	{
		const auto generalElementOffset = 5.0;
		const auto verticalScreenBorderOffset = 100.0;
		const auto optionsXOffset = 50.0;
		const auto checkboxYOffset = 10.0;
		const auto checkboxYGroupOffset = 32.0;

		optionsMenuTitle->setPosition(getScreenWidth() / 2.0, verticalScreenBorderOffset / 2.0);

		applyButton->setPosition(getScreenWidth() / 2.0, getScreenHeight() - verticalScreenBorderOffset);
		resetButton->toRightOf(applyButton, generalElementOffset, 0.0);
		backButton->toLeftOf(applyButton, generalElementOffset, 0.0);

		resolutionScrollArea->centerAlignSelf(generalElementOffset + (getScreenWidth() - 651.0) / 2.0, (getScreenHeight() - 600.0) / 2.0);
		resolutionScrollArea->floorAlignSelf();
		{
			auto r = resolutionScrollArea->getRect();
			resolutionScrollArea->setClipRect(MT::Rect(0.0, 0.0, r.w + 30.0, r.h));
		}

		fullscreenCheckbox->toRightTopOf(resolutionScrollArea, optionsXOffset);
		fullscreenDesktopCheckbox->toBottomLeftOf(fullscreenCheckbox, 0.0, checkboxYOffset);
		windowedCheckbox->toBottomLeftOf(fullscreenDesktopCheckbox, 0.0, checkboxYOffset);

		msaaOffCheckbox->toBottomLeftOf(windowedCheckbox, 0.0, checkboxYOffset + checkboxYGroupOffset);
		msaa4xCheckbox->toBottomLeftOf(msaaOffCheckbox, 0.0, checkboxYOffset);
		msaa8xCheckbox->toBottomLeftOf(msaa4xCheckbox, 0.0, checkboxYOffset);
		msaa16xCheckbox->toBottomLeftOf(msaa8xCheckbox, 0.0, checkboxYOffset);

		openGlCompatibilityModeCheckbox->toBottomLeftOf(msaa16xCheckbox, 0.0, checkboxYOffset + checkboxYGroupOffset);
		wireframeModeCheckbox->toBottomLeftOf(openGlCompatibilityModeCheckbox, 0.0, checkboxYOffset);
	}

	void SceneOptionsMenu::onButtonClicked(int id)
	{
		if (id == backButton->getId())
		{
			const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
			if (mainGameScene != nullptr)
			{
				mainGameScene->hideOptions();
			}
			else
			{
				transitionToScene(SceneGame::MainMenu);
			}
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

		if (id == msaa4xCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(4);
		}

		if (id == msaa8xCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(8);
		}

		if (id == msaa16xCheckbox->getId())
		{
			shouldEnableApply = setMsaaMode(16);
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

		const auto containerUpperBound = resolutionScrollArea->getScreenRect()->y;
		const auto containerLowerBound = resolutionScrollArea->getScreenRect()->y + resolutionScrollArea->getScreenRect()->h;
		for (const auto resolutionButton : resolutionButtons)
		{
			const auto buttonUpperBound = resolutionButton->getScreenRect()->y;
			const auto buttonLowerBound = resolutionButton->getScreenRect()->y + resolutionButton->getScreenRect()->h;
			if (resolutionButton->getId() == id && buttonUpperBound <= containerLowerBound && buttonLowerBound >= containerUpperBound)
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

	void SceneOptionsMenu::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_RETURN:
			if (applyButton->getEnabled() == true)
			{
				modules->event->pushEvent(std::make_shared<MT::ReprovisionScreenApplicationEvent>(config));
				applyButton->setEnabled(false);
			}
			break;
		}
	}

	bool SceneOptionsMenu::setMsaaMode(int samples)
	{
		if (config.msaaSamples == samples)
		{
			return false;
		}

		msaaOffCheckbox->setChecked(false);
		msaa4xCheckbox->setChecked(false);
		msaa8xCheckbox->setChecked(false);
		msaa16xCheckbox->setChecked(false);

		switch (samples)
		{
		case 4:
			msaa4xCheckbox->setChecked(true);
			break;

		case 8:
			msaa8xCheckbox->setChecked(true);
			break;

		case 16:
			msaa16xCheckbox->setChecked(true);
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