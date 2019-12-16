#include "SceneOptionsMenu.h"
#include "scene/game/SceneMainGame.h"

namespace
{
	const auto testMusicName = "tetris-music";
	const auto testSoundName = "tetris-clear-row";
	const auto fontSize = 24;
	const std::vector<int> frameLimits = {
		0,
		30,
		60,
		90,
		120,
		144,
		165,
		200,
		240
	};
}

namespace AWGame
{
	SceneOptionsMenu::SceneOptionsMenu() : BaseScene(SceneGame::OptionsMenu)
	{
		rebuildOnLoad = true;
		enableSerialization<SceneOptionsMenu>();
	}

	void SceneOptionsMenu::onInitialAttach()
	{
		modules->input->keyboard->registerKeys({ SDL_SCANCODE_RETURN, SDL_SCANCODE_BACKSPACE }, weak_from_this());
	}

	void SceneOptionsMenu::onDetach()
	{
		modules->sound->stopAllSounds(250);
	}

	void SceneOptionsMenu::onDestroyChildren()
	{
		resolutionButtons.clear();
	}

	void SceneOptionsMenu::onCreateChildren()
	{
		info = modules->screen->getAllSupportedDisplayModes();
		config = modules->screen->getCurrentScreenConfig();

		optionsMenuTitle = std::make_shared<AWCore::Text>();
		optionsMenuTitle->setFont("medium", fontSize);
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
			if (AWCore::StringHelper::getDisplayComponentForDisplayString(resolution, 0) < 1240 || AWCore::StringHelper::getDisplayComponentForDisplayString(resolution, 1) < 1024)
			{
				continue;
			}

			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			if (config.mode == AWCore::ScreenModes::FullscreenDesktop || resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
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
		fullscreenCheckbox->setChecked(config.mode == AWCore::ScreenModes::Fullscreen);
		fullscreenCheckbox->clickListener = weak_from_this();
		add(fullscreenCheckbox);

		fullscreenDesktopCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenDesktopCheckbox->setText("Fullscreen Windowed");
		fullscreenDesktopCheckbox->setChecked(config.mode == AWCore::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->clickListener = weak_from_this();
		add(fullscreenDesktopCheckbox);

		windowedCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		windowedCheckbox->setText("Window");
		windowedCheckbox->setChecked(config.mode == AWCore::ScreenModes::Windowed);
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

		vsyncOffCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		vsyncOffCheckbox->setText("VSync Off");
		vsyncOffCheckbox->setChecked(config.vMode == AWCore::VsyncModes::Disabled);
		vsyncOffCheckbox->clickListener = weak_from_this();
		add(vsyncOffCheckbox);

		vsyncOnCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		vsyncOnCheckbox->setText("VSync On");
		vsyncOnCheckbox->setChecked(config.vMode == AWCore::VsyncModes::Enabled);
		vsyncOnCheckbox->clickListener = weak_from_this();
		add(vsyncOnCheckbox);

		vsyncAdaptiveCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		vsyncAdaptiveCheckbox->setText("VSync Adaptive");
		vsyncAdaptiveCheckbox->setChecked(config.vMode == AWCore::VsyncModes::Adaptive);
		vsyncAdaptiveCheckbox->clickListener = weak_from_this();
		add(vsyncAdaptiveCheckbox);

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

		debugRenderingCheckbox = std::make_shared<CheckBoxBasic>();
		debugRenderingCheckbox->setText("Debug Rendering");
		debugRenderingCheckbox->setChecked(config.visualizeContainers || config.visualizeClipRects);
		debugRenderingCheckbox->clickListener = weak_from_this();
		add(debugRenderingCheckbox);

		masterVolScrollBar = std::make_shared<ScrollBarBasic>();
		masterVolScrollBar->setSize(200.0, 20.0);
		masterVolScrollBar->setHorizontal(true);
		masterVolScrollBar->scrollListener = weak_from_this();
		add(masterVolScrollBar);

		generalVolScrollBar = std::make_shared<ScrollBarBasic>();
		generalVolScrollBar->setSize(200.0, 20.0);
		generalVolScrollBar->setHorizontal(true);
		generalVolScrollBar->scrollListener = weak_from_this();
		add(generalVolScrollBar);

		musicVolScrollBar = std::make_shared<ScrollBarBasic>();
		musicVolScrollBar->setSize(200.0, 20.0);
		musicVolScrollBar->setHorizontal(true);
		musicVolScrollBar->scrollListener = weak_from_this();
		add(musicVolScrollBar);

		frameLimiterScrollBar = std::make_shared<ScrollBarBasic>();
		frameLimiterScrollBar->setSize(200.0, 20.0);
		frameLimiterScrollBar->setHorizontal(true);
		frameLimiterScrollBar->clickListener = weak_from_this();
		frameLimiterScrollBar->scrollListener = weak_from_this();
		add(frameLimiterScrollBar);

		masterVolLabel = std::make_shared<AWCore::Text>();
		masterVolLabel->setFont("medium", fontSize);
		add(masterVolLabel);

		generalVolLabel = std::make_shared<AWCore::Text>();
		generalVolLabel->setFont("medium", fontSize);
		add(generalVolLabel);

		musicVolLabel = std::make_shared<AWCore::Text>();
		musicVolLabel->setFont("medium", fontSize);
		add(musicVolLabel);

		frameLimiterLabelPrefix = std::make_shared<AWCore::Text>();
		frameLimiterLabelPrefix->setFont("medium", fontSize);
		frameLimiterLabelPrefix->setText("Frame Limiter ");
		add(frameLimiterLabelPrefix);

		frameLimiterLabelValue = std::make_shared<AWCore::Text>();
		frameLimiterLabelValue->setFont("medium", fontSize);
		add(frameLimiterLabelValue);

		masterVolScrollBar->setScrollPositionInstantly(modules->sound->getMasterVolume());
		generalVolScrollBar->setScrollPositionInstantly(modules->sound->getEffectVolume());
		musicVolScrollBar->setScrollPositionInstantly(modules->sound->getMusicVolume());
		frameLimiterScrollBar->setScrollPositionInstantly(frameLimitToPosition(config.frameLimiter));

		setDynamicLabels();
	}

	void SceneOptionsMenu::onLayoutChildren()
	{
		const auto generalElementOffset = 5.0;
		const auto verticalScreenBorderOffset = 100.0;
		const auto optionsXOffset = 40.0;
		const auto checkboxYOffset = 10.0;
		const auto checkboxYGroupOffset = 32.0;

		optionsMenuTitle->setPosition(getScreenWidth() / 2.0, verticalScreenBorderOffset / 2.0);

		applyButton->setPosition(getScreenWidth() / 2.0, getScreenHeight() - verticalScreenBorderOffset);
		resetButton->toRightOf(applyButton, generalElementOffset, 0.0);
		backButton->toLeftOf(applyButton, generalElementOffset, 0.0);

		resolutionScrollArea->centerAlignSelf(generalElementOffset + (getScreenWidth() - 859.0) / 2.0, (getScreenHeight() - 600.0) / 2.0);
		resolutionScrollArea->floorAlignSelf();
		{
			auto r = resolutionScrollArea->getRect();
			resolutionScrollArea->setClipRect(AWCore::Rect(0.0, 0.0, r.w + 30.0, r.h));
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
		debugRenderingCheckbox->toBottomLeftOf(wireframeModeCheckbox, 0.0, checkboxYOffset);

		vsyncOffCheckbox->toRightOf(fullscreenCheckbox, 405.0);
		vsyncOnCheckbox->toBottomLeftOf(vsyncOffCheckbox, 0.0, checkboxYOffset);
		vsyncAdaptiveCheckbox->toBottomLeftOf(vsyncOnCheckbox, 0.0, checkboxYOffset);

		frameLimiterLabelPrefix->toBottomLeftOf(vsyncAdaptiveCheckbox, 0.0, checkboxYGroupOffset);
		frameLimiterLabelValue->toRightOf(frameLimiterLabelPrefix);
		frameLimiterScrollBar->toBottomLeftOf(frameLimiterLabelPrefix, 0.0, checkboxYOffset);

		masterVolLabel->toBottomLeftOf(frameLimiterScrollBar, 0.0, checkboxYGroupOffset);
		masterVolScrollBar->toBottomLeftOf(masterVolLabel, 0.0, checkboxYOffset);

		generalVolLabel->toBottomLeftOf(masterVolScrollBar, 0.0, checkboxYOffset);
		generalVolScrollBar->toBottomLeftOf(generalVolLabel, 0.0, checkboxYOffset);

		musicVolLabel->toBottomLeftOf(generalVolScrollBar, 0.0, checkboxYOffset);
		musicVolScrollBar->toBottomLeftOf(musicVolLabel, 0.0, checkboxYOffset);
	}

	void SceneOptionsMenu::onTimeoutCalled(int id)
	{
		if (id == stopSoundTimeoutId && isAttached())
		{
			modules->sound->stopMusic(testMusicName);
			stopSoundTimeoutId = 0;
		}
		else if (id == playSoundTimeoutId)
		{
			modules->sound->playSoundClip(testSoundName);
			playSoundTimeoutId = 0;
		}
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

		if (id == applyButton->getId())
		{
			shouldNotifyApplication = true;
		}

		if (id == fullscreenCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(AWCore::ScreenModes::Fullscreen);
		}

		if (id == windowedCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(AWCore::ScreenModes::Windowed);
		}

		if (id == fullscreenDesktopCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(AWCore::ScreenModes::FullscreenDesktop);
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

		if (id == vsyncOffCheckbox->getId())
		{
			shouldEnableApply = setVsyncMode(AWCore::VsyncModes::Disabled);
		}

		if (id == vsyncOnCheckbox->getId())
		{
			shouldEnableApply = setVsyncMode(AWCore::VsyncModes::Enabled);
		}

		if (id == vsyncAdaptiveCheckbox->getId())
		{
			shouldEnableApply = setVsyncMode(AWCore::VsyncModes::Adaptive);
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

		if (id == debugRenderingCheckbox->getId())
		{
			config.visualizeClipRects = !(config.visualizeClipRects || config.visualizeContainers);
			config.visualizeContainers = config.visualizeClipRects;
			debugRenderingCheckbox->setChecked(config.visualizeClipRects);
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
				config.width = AWCore::StringHelper::getDisplayComponentForDisplayString(&newResolution, 0);
				config.height = AWCore::StringHelper::getDisplayComponentForDisplayString(&newResolution, 1);
				shouldEnableApply = true;
			}
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (config.mode == AWCore::ScreenModes::FullscreenDesktop || (resolutionButton->getText() == std::to_string(config.width) + "x" + std::to_string(config.height)))
			{
				resolutionButton->setEnabled(false);
			}
			else
			{
				resolutionButton->setEnabled(true);
			}
		}

		if (id == frameLimiterScrollBar->getId())
		{
			frameLimiterScrollBar->setScrollPosition(frameLimitToPosition(config.frameLimiter));
		}

		if (id == resetButton->getId())
		{
			config = AWCore::ScreenConfig();
			modules->sound->setMasterVolume(1.0);
			modules->sound->setEffectVolume(0.8);
			modules->sound->setMusicVolume(0.6);
			shouldNotifyApplication = true;
		}

		if (shouldEnableApply)
		{
			applyButton->setEnabled(true);
		}

		if (shouldNotifyApplication)
		{
			modules->event->pushEvent(std::make_shared<AWCore::ReprovisionScreenApplicationEvent>(config));
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
				modules->event->pushEvent(std::make_shared<AWCore::ReprovisionScreenApplicationEvent>(config));
				applyButton->setEnabled(false);
			}
			break;

		case SDL_SCANCODE_BACKSPACE:
			config = AWCore::ScreenConfig();
			modules->sound->setMasterVolume(1.0);
			modules->sound->setEffectVolume(0.8);
			modules->sound->setMusicVolume(0.6);
			modules->event->pushEvent(std::make_shared<AWCore::ReprovisionScreenApplicationEvent>(config));
			applyButton->setEnabled(false);
		}
	}

	void SceneOptionsMenu::onScrollBarScroll(int id, double position)
	{
		auto playEffectSound = false;
		auto shouldMuteSound = false;
		if (id == masterVolScrollBar->getId() && modules->sound->getMasterVolume() != position)
		{
			modules->sound->setMasterVolume(position);
			modules->sound->playMusic(testMusicName);
			setDynamicLabels();

			playEffectSound = true;
			shouldMuteSound = true;
		}
		else if (id == generalVolScrollBar->getId() && modules->sound->getEffectVolume() != position)
		{
			modules->sound->setEffectVolume(position);
			modules->sound->stopMusic(testMusicName);
			setDynamicLabels();

			playEffectSound = true;
			shouldMuteSound = true;
		}
		else if (id == musicVolScrollBar->getId() && modules->sound->getMusicVolume() != position)
		{
			modules->sound->setMusicVolume(position);
			modules->sound->playMusic(testMusicName);
			setDynamicLabels();
			shouldMuteSound = true;
		}
		else if (id == frameLimiterScrollBar->getId() && config.frameLimiter != positionToFrameLimit(position))
		{
			config.frameLimiter = positionToFrameLimit(position);
			applyButton->setEnabled(true);
			setDynamicLabels();
		}

		if (shouldMuteSound)
		{
			setTimeout(2000.0, &stopSoundTimeoutId);
		}

		if (playEffectSound)
		{
			setTimeout(500.0, &playSoundTimeoutId);
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

	bool SceneOptionsMenu::setScreenMode(AWCore::ScreenModes mode)
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
		case AWCore::ScreenModes::Fullscreen:
			fullscreenCheckbox->setChecked(true);
			break;

		case AWCore::ScreenModes::Windowed:
			windowedCheckbox->setChecked(true);
			break;

		case AWCore::ScreenModes::FullscreenDesktop:
			fullscreenDesktopCheckbox->setChecked(true);
			break;
		}

		config.mode = mode;

		return true;
	}

	bool SceneOptionsMenu::setVsyncMode(AWCore::VsyncModes mode)
	{
		if (config.vMode == mode)
		{
			return false;
		}

		vsyncOffCheckbox->setChecked(false);
		vsyncOnCheckbox->setChecked(false);
		vsyncAdaptiveCheckbox->setChecked(false);

		switch (mode)
		{
		case AWCore::VsyncModes::Disabled:
			vsyncOffCheckbox->setChecked(true);
			break;

		case AWCore::VsyncModes::Enabled:
			vsyncOnCheckbox->setChecked(true);
			break;

		case AWCore::VsyncModes::Adaptive:
			vsyncAdaptiveCheckbox->setChecked(true);
			break;
		}

		config.vMode = mode;

		return true;

	}

	void SceneOptionsMenu::setDynamicLabels()
	{
		masterVolLabel->setText("Master Volume " + volumeToString(modules->sound->getMasterVolume()));
		generalVolLabel->setText("Effect Volume " + volumeToString(modules->sound->getEffectVolume()));
		musicVolLabel->setText("Music Volume " + volumeToString(modules->sound->getMusicVolume()));
		if (config.frameLimiter < 30)
		{
			frameLimiterLabelValue->setText("Off");
		}
		else
		{
			frameLimiterLabelValue->setText(std::to_string(config.frameLimiter) + " fps");
		}

		layout();
	}

	std::string SceneOptionsMenu::volumeToString(double volume)
	{
		return std::to_string((int)(volume * 100.0));
	}

	int SceneOptionsMenu::positionToFrameLimit(double position)
	{
		const int i = std::ceil(position * (frameLimits.size() - 1));
		return frameLimits[i];
	}

	double SceneOptionsMenu::frameLimitToPosition(int limit)
	{
		auto i = 0;
		for (const auto lim : frameLimits)
		{
			if (lim == limit)
			{
				break;
			}

			++i;
		}

		return i / (double)frameLimits.size();
	}

}