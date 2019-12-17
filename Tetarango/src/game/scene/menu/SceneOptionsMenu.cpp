#include "SceneOptionsMenu.h"
#include "scene/game/SceneMainGame.h"

namespace
{
	const auto testMusicName = "tetris-music";
	const auto testSoundName = "tetris-clear-row";
	const auto headerFontSize = 28;
	const auto itemFontSize = 24;
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
		registerSerialization<SceneOptionsMenu>();
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

		rootContainer = std::make_shared<AW::Container>();
		rootContainer->renderPositionProcessing = AW::RenderPositionProcessing::Floor;
		rootContainer->setSizeToScreenSize();
		rootContainer->topLeftAlignSelf();
		add(rootContainer);

		centeringContainer = std::make_shared<AW::Container>();
		centeringContainer->matchSizeAndCenter(rootContainer);
		rootContainer->add(centeringContainer);

		optionsMenuTitle = std::make_shared<AW::Text>();
		optionsMenuTitle->setFont("medium", headerFontSize);
		optionsMenuTitle->setText("Options");
		rootContainer->add(optionsMenuTitle);

		applyButton = std::make_shared<ButtonBasic>();
		applyButton->setText("Apply");
		applyButton->clickListener = weak_from_this();
		applyButton->setEnabled(false);
		rootContainer->add(applyButton);

		resetButton = std::make_shared<ButtonBasic>();
		resetButton->setText("Reset");
		resetButton->clickListener = weak_from_this();
		rootContainer->add(resetButton);

		backButton = std::make_shared<ButtonBasic>();
		backButton->setText("Back");
		backButton->clickListener = weak_from_this();
		rootContainer->add(backButton);

		resolutionScrollArea = std::make_shared<ScrollArea>();
		centeringContainer->add(resolutionScrollArea);

		resolutionButtons.clear();
		std::shared_ptr<ButtonBasic> prevResolutionButton;
		for (const auto resolution : info.resolutions)
		{
			if (AW::StringHelper::getDisplayComponentForDisplayString(resolution, 0) < 1240 || AW::StringHelper::getDisplayComponentForDisplayString(resolution, 1) < 1024)
			{
				continue;
			}

			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			resolutionButton->setSize(180.0, 60.0);
			resolutionButton->setFontSize(24.0);
			if (config.mode == AW::ScreenModes::FullscreenDesktop || resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
			{
				resolutionButton->setEnabled(false);
			}

			resolutionButton->clickListener = weak_from_this();
			resolutionButtons.push_back(resolutionButton);

			if (prevResolutionButton != nullptr)
			{
				resolutionButton->toBottomOf(prevResolutionButton, 0, 6.0);
			}
			else
			{
				resolutionButton->topLeftAlignSelf();
			}

			prevResolutionButton = resolutionButton;
			resolutionScrollArea->add(resolutionButton);
		}

		fullscreenCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenCheckbox->setText("Fullscreen");
		fullscreenCheckbox->setChecked(config.mode == AW::ScreenModes::Fullscreen);
		fullscreenCheckbox->clickListener = weak_from_this();
		centeringContainer->add(fullscreenCheckbox);

		fullscreenDesktopCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		fullscreenDesktopCheckbox->setText("Fullscreen Windowed");
		fullscreenDesktopCheckbox->setChecked(config.mode == AW::ScreenModes::FullscreenDesktop);
		fullscreenDesktopCheckbox->clickListener = weak_from_this();
		centeringContainer->add(fullscreenDesktopCheckbox);

		windowedCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		windowedCheckbox->setText("Window");
		windowedCheckbox->setChecked(config.mode == AW::ScreenModes::Windowed);
		windowedCheckbox->clickListener = weak_from_this();
		centeringContainer->add(windowedCheckbox);

		msaaOffCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaaOffCheckbox->setText("Off");
		msaaOffCheckbox->setChecked(config.msaaSamples == 0);
		msaaOffCheckbox->clickListener = weak_from_this();
		centeringContainer->add(msaaOffCheckbox);

		msaa4xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa4xCheckbox->setText("4x");
		msaa4xCheckbox->setChecked(config.msaaSamples == 4);
		msaa4xCheckbox->clickListener = weak_from_this();
		centeringContainer->add(msaa4xCheckbox);

		msaa8xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa8xCheckbox->setText("8x");
		msaa8xCheckbox->setChecked(config.msaaSamples == 8);
		msaa8xCheckbox->clickListener = weak_from_this();
		centeringContainer->add(msaa8xCheckbox);

		msaa16xCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		msaa16xCheckbox->setText("16x");
		msaa16xCheckbox->setChecked(config.msaaSamples == 16);
		msaa16xCheckbox->clickListener = weak_from_this();
		centeringContainer->add(msaa16xCheckbox);

		vsyncOffCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		vsyncOffCheckbox->setText("Off");
		vsyncOffCheckbox->setChecked(config.vMode == AW::VsyncModes::Disabled);
		vsyncOffCheckbox->clickListener = weak_from_this();
		centeringContainer->add(vsyncOffCheckbox);

		vsyncOnCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		vsyncOnCheckbox->setText("On");
		vsyncOnCheckbox->setChecked(config.vMode == AW::VsyncModes::Enabled);
		vsyncOnCheckbox->clickListener = weak_from_this();
		centeringContainer->add(vsyncOnCheckbox);

		vsyncAdaptiveCheckbox = std::make_shared<CheckBoxBasic>(GuiButton::RadioBoxBasic);
		vsyncAdaptiveCheckbox->setText("Adaptive");
		vsyncAdaptiveCheckbox->setChecked(config.vMode == AW::VsyncModes::Adaptive);
		vsyncAdaptiveCheckbox->clickListener = weak_from_this();
		centeringContainer->add(vsyncAdaptiveCheckbox);

		openGlCompatibilityModeCheckbox = std::make_shared<CheckBoxBasic>();
		openGlCompatibilityModeCheckbox->setText("OpenGL Compatibility Mode");
		openGlCompatibilityModeCheckbox->setChecked(config.openGLCompatibilityMode);
		openGlCompatibilityModeCheckbox->clickListener = weak_from_this();
		centeringContainer->add(openGlCompatibilityModeCheckbox);

		wireframeModeCheckbox = std::make_shared<CheckBoxBasic>();
		wireframeModeCheckbox->setText("Wireframe Mode");
		wireframeModeCheckbox->setChecked(config.openGlWireframeMode);
		wireframeModeCheckbox->clickListener = weak_from_this();
		centeringContainer->add(wireframeModeCheckbox);

		debugRenderingCheckbox = std::make_shared<CheckBoxBasic>();
		debugRenderingCheckbox->setText("Debug Rendering");
		debugRenderingCheckbox->setChecked(config.visualizeContainers || config.visualizeClipRects);
		debugRenderingCheckbox->clickListener = weak_from_this();
		centeringContainer->add(debugRenderingCheckbox);

		masterVolScrollBar = std::make_shared<ScrollBarBasic>();
		masterVolScrollBar->setSize(200.0, 30.0);
		masterVolScrollBar->setHorizontal(true);
		masterVolScrollBar->scrollListener = weak_from_this();
		centeringContainer->add(masterVolScrollBar);

		generalVolScrollBar = std::make_shared<ScrollBarBasic>();
		generalVolScrollBar->setSize(200.0, 30.0);
		generalVolScrollBar->setHorizontal(true);
		generalVolScrollBar->scrollListener = weak_from_this();
		centeringContainer->add(generalVolScrollBar);

		musicVolScrollBar = std::make_shared<ScrollBarBasic>();
		musicVolScrollBar->setSize(200.0, 30.0);
		musicVolScrollBar->setHorizontal(true);
		musicVolScrollBar->scrollListener = weak_from_this();
		centeringContainer->add(musicVolScrollBar);

		frameLimiterScrollBar = std::make_shared<ScrollBarBasic>();
		frameLimiterScrollBar->setSize(200.0, 30.0);
		frameLimiterScrollBar->setHorizontal(true);
		frameLimiterScrollBar->clickListener = weak_from_this();
		frameLimiterScrollBar->scrollListener = weak_from_this();
		centeringContainer->add(frameLimiterScrollBar);

		resolutionsLabel = std::make_shared<AW::Text>();
		resolutionsLabel->setFont("medium", headerFontSize);
		resolutionsLabel->setText("Resolutions");
		centeringContainer->add(resolutionsLabel);

		msaaLabel = std::make_shared<AW::Text>();
		msaaLabel->setFont("medium", headerFontSize);
		msaaLabel->setText("Multisample Anti-Aliasing");
		centeringContainer->add(msaaLabel);

		vsyncLabel = std::make_shared<AW::Text>();
		vsyncLabel->setFont("medium", headerFontSize);
		vsyncLabel->setText("VSync");
		centeringContainer->add(vsyncLabel);

		miscLabel = std::make_shared<AW::Text>();
		miscLabel->setFont("medium", headerFontSize);
		miscLabel->setText("Misc");
		centeringContainer->add(miscLabel);

		displayModeLabel = std::make_shared<AW::Text>();
		displayModeLabel->setFont("medium", headerFontSize);
		displayModeLabel->setText("Display Mode");
		centeringContainer->add(displayModeLabel);

		masterVolLabel = std::make_shared<AW::Text>();
		masterVolLabel->setFont("medium", itemFontSize);
		centeringContainer->add(masterVolLabel);

		soundLabel = std::make_shared<AW::Text>();
		soundLabel->setFont("medium", headerFontSize);
		soundLabel->setText("Sound");
		centeringContainer->add(soundLabel);

		generalVolLabel = std::make_shared<AW::Text>();
		generalVolLabel->setFont("medium", itemFontSize);
		centeringContainer->add(generalVolLabel);

		musicVolLabel = std::make_shared<AW::Text>();
		musicVolLabel->setFont("medium", itemFontSize);
		centeringContainer->add(musicVolLabel);

		frameLimiterLabelPrefix = std::make_shared<AW::Text>();
		frameLimiterLabelPrefix->setFont("medium", headerFontSize);
		frameLimiterLabelPrefix->setText("Frame Limiter ");
		centeringContainer->add(frameLimiterLabelPrefix);

		frameLimiterLabelValue = std::make_shared<AW::Text>();
		frameLimiterLabelValue->setFont("medium", headerFontSize);
		centeringContainer->add(frameLimiterLabelValue);

		masterVolScrollBar->setScrollPositionInstantly(modules->sound->getMasterVolume());
		generalVolScrollBar->setScrollPositionInstantly(modules->sound->getEffectVolume());
		musicVolScrollBar->setScrollPositionInstantly(modules->sound->getMusicVolume());
		frameLimiterScrollBar->setScrollPositionInstantly(frameLimitToPosition(config.frameLimiter));

		setDynamicLabels();
	}

	void SceneOptionsMenu::onLayoutChildren()
	{
		const auto generalElementOffset = 6.0;
		const auto verticalScreenBorderOffset = 100.0;
		const auto optionsXOffset = 40.0;
		const auto checkboxYOffset = 8.0;
		const auto checkboxYGroupOffset = 32.0;
		const auto numResolutionButtonsToShow = 7.0;
		const auto horScreenPadding = openGlCompatibilityModeCheckbox->getHalfWidth() + 50.0;

		resolutionsLabel->setPosition(0.0, 0.0);
		if (!resolutionButtons.empty())
		{
			const auto exampleButton = resolutionButtons.front();
			const auto availableVerticalScreen = applyButton->getY() - optionsMenuTitle->getY();
			resolutionScrollArea->visible = true;
			resolutionScrollArea->setScrollAmount(exampleButton->getHalfHeight());
			resolutionScrollArea->setSize(exampleButton->getWidth(), (exampleButton->getHeight() + generalElementOffset) * numResolutionButtonsToShow);
			resolutionScrollArea->setScrollerHeight(resolutionScrollArea->getHeight());

			resolutionScrollArea->toBottomOf(resolutionsLabel, 0.0, generalElementOffset);
			{
				auto r = resolutionScrollArea->getRect();
				resolutionScrollArea->setClipRect(AW::Rect(0.0, 0.0, r.w + 30.0, r.h));
			}
		}
		else
		{
			resolutionScrollArea->visible = false;
		}

		displayModeLabel->toRightOf(resolutionsLabel, std::floor(optionsXOffset + resolutionScrollArea->getWidth() - resolutionsLabel->getWidth()));
		fullscreenCheckbox->toBottomLeftOf(displayModeLabel, 0.0, checkboxYOffset);
		fullscreenDesktopCheckbox->toBottomLeftOf(fullscreenCheckbox, 0.0, checkboxYOffset);
		windowedCheckbox->toBottomLeftOf(fullscreenDesktopCheckbox, 0.0, checkboxYOffset);

		msaaLabel->toBottomLeftOf(windowedCheckbox, 0.0, checkboxYGroupOffset);
		msaaOffCheckbox->toBottomLeftOf(msaaLabel, 0.0, checkboxYOffset);
		msaa4xCheckbox->toRightOf(msaaOffCheckbox, checkboxYOffset);
		msaa8xCheckbox->toRightOf(msaa4xCheckbox, checkboxYOffset);
		msaa16xCheckbox->toRightOf(msaa8xCheckbox, checkboxYOffset);

		vsyncLabel->toBottomLeftOf(msaaOffCheckbox, 0.0, checkboxYGroupOffset);
		vsyncOffCheckbox->toBottomRightOf(vsyncLabel, 0.0, checkboxYOffset);
		vsyncOnCheckbox->toRightOf(vsyncOffCheckbox, checkboxYOffset);
		vsyncAdaptiveCheckbox->toRightOf(vsyncOnCheckbox, checkboxYOffset);

		frameLimiterLabelPrefix->toBottomLeftOf(vsyncOffCheckbox, 0.0, checkboxYGroupOffset);
		frameLimiterLabelValue->toRightOf(frameLimiterLabelPrefix);
		frameLimiterScrollBar->toBottomLeftOf(frameLimiterLabelPrefix, 0.0, checkboxYOffset);

		miscLabel->toRightOf(displayModeLabel, std::floor(optionsXOffset + (msaa16xCheckbox->getRight() - msaaOffCheckbox->getLeft()) - displayModeLabel->getWidth()), 0.0);
		openGlCompatibilityModeCheckbox->toBottomLeftOf(miscLabel, 0.0, checkboxYOffset);
		wireframeModeCheckbox->toBottomLeftOf(openGlCompatibilityModeCheckbox, 0.0, checkboxYOffset);
		debugRenderingCheckbox->toBottomLeftOf(wireframeModeCheckbox, 0.0, checkboxYOffset);

		soundLabel->toBottomLeftOf(debugRenderingCheckbox, 0.0, checkboxYGroupOffset);
		masterVolLabel->toBottomLeftOf(soundLabel, 0.0, checkboxYOffset);
		masterVolScrollBar->toBottomLeftOf(masterVolLabel, 0.0, checkboxYOffset);

		generalVolLabel->toBottomLeftOf(masterVolScrollBar, 0.0, checkboxYOffset);
		generalVolScrollBar->toBottomLeftOf(generalVolLabel, 0.0, checkboxYOffset);

		musicVolLabel->toBottomLeftOf(generalVolScrollBar, 0.0, checkboxYOffset);
		musicVolScrollBar->toBottomLeftOf(musicVolLabel, 0.0, checkboxYOffset);

		centeringContainer->resizeSelfToChildrenAndCenterChildren();

		applyButton->toBottomOf(centeringContainer, 0.0, verticalScreenBorderOffset);
		resetButton->toRightOf(applyButton, generalElementOffset, 0.0);
		backButton->toLeftOf(applyButton, generalElementOffset, 0.0);

		optionsMenuTitle->toTopOf(centeringContainer, 0.0, verticalScreenBorderOffset);
	}

	void SceneOptionsMenu::onTransitionedTo()
	{
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
			shouldEnableApply = setScreenMode(AW::ScreenModes::Fullscreen);
		}

		if (id == windowedCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(AW::ScreenModes::Windowed);
		}

		if (id == fullscreenDesktopCheckbox->getId())
		{
			shouldEnableApply = setScreenMode(AW::ScreenModes::FullscreenDesktop);
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
			shouldEnableApply = setVsyncMode(AW::VsyncModes::Disabled);
		}

		if (id == vsyncOnCheckbox->getId())
		{
			shouldEnableApply = setVsyncMode(AW::VsyncModes::Enabled);
		}

		if (id == vsyncAdaptiveCheckbox->getId())
		{
			shouldEnableApply = setVsyncMode(AW::VsyncModes::Adaptive);
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
				config.width = AW::StringHelper::getDisplayComponentForDisplayString(&newResolution, 0);
				config.height = AW::StringHelper::getDisplayComponentForDisplayString(&newResolution, 1);
				shouldEnableApply = true;
			}
		}

		for (const auto resolutionButton : resolutionButtons)
		{
			if (config.mode == AW::ScreenModes::FullscreenDesktop || (resolutionButton->getText() == std::to_string(config.width) + "x" + std::to_string(config.height)))
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
			config = AW::ScreenConfig();
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
			modules->event->pushEvent(std::make_shared<AW::ReprovisionScreenApplicationEvent>(config));
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
				modules->event->pushEvent(std::make_shared<AW::ReprovisionScreenApplicationEvent>(config));
				applyButton->setEnabled(false);
			}
			break;

		case SDL_SCANCODE_BACKSPACE:
			config = AW::ScreenConfig();
			modules->sound->setMasterVolume(1.0);
			modules->sound->setEffectVolume(0.8);
			modules->sound->setMusicVolume(0.6);
			modules->event->pushEvent(std::make_shared<AW::ReprovisionScreenApplicationEvent>(config));
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

	bool SceneOptionsMenu::setScreenMode(AW::ScreenModes mode)
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
		case AW::ScreenModes::Fullscreen:
			fullscreenCheckbox->setChecked(true);
			break;

		case AW::ScreenModes::Windowed:
			windowedCheckbox->setChecked(true);
			break;

		case AW::ScreenModes::FullscreenDesktop:
			fullscreenDesktopCheckbox->setChecked(true);
			break;
		}

		config.mode = mode;

		return true;
	}

	bool SceneOptionsMenu::setVsyncMode(AW::VsyncModes mode)
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
		case AW::VsyncModes::Disabled:
			vsyncOffCheckbox->setChecked(true);
			break;

		case AW::VsyncModes::Enabled:
			vsyncOnCheckbox->setChecked(true);
			break;

		case AW::VsyncModes::Adaptive:
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