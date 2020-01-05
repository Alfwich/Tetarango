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
#if _DEBUG
		10,
		20,
		30,
#endif // _DEBUG
		60,
		90,
		120,
		144,
		165,
		200,
		240
	};
	const auto resolutionButtonSelectedColor = AW::Color(255, 225, 255);
	const auto resolutionButtonDisabledColor = AW::Color(64, 64, 64);
	const auto resolutionButtonColor = AW::Color(192, 192, 192);

	const auto applyButtonColor = AW::Color(255, 255, 255);
	const auto applyButtonDisabledColor = AW::Color(64, 64, 64);
}

namespace AWGame
{
	SceneOptionsMenu::SceneOptionsMenu() : BaseScene(SceneGame::OptionsMenu)
	{
		setShouldRebuildOnLoad();
		GORegister(SceneOptionsMenu);
	}

	void SceneOptionsMenu::onInitialAttach()
	{
		modules->input->keyboard->registerKeys({ AWKey::RETURN, AWKey::BACKSPACE }, weak_from_this());
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
		centeringContainer->setSizeAndCenter(rootContainer);
		rootContainer->add(centeringContainer);

		optionsMenuTitle = std::make_shared<AW::Text>();
		optionsMenuTitle->setFont("medium", headerFontSize);
		optionsMenuTitle->setText("Options");
		rootContainer->add(optionsMenuTitle);

		applyButton = std::make_shared<ButtonBasic>();
		applyButton->setText("Apply");
		applyButton->clickListener = weak_from_this();
		applyButton->setEnabled(false);
		applyButton->setColor(applyButtonDisabledColor);
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
		const auto maxWindowedWidth = modules->screen->getMaxWindowedWidth();
		const auto maxWindowedHeight = modules->screen->getMaxWindowedHeight();
		for (const auto resolution : info.resolutions)
		{
			auto resolutionButton = std::make_shared<ButtonBasic>();
			resolutionButton->setText(resolution);
			resolutionButton->setSize(180.0, 60.0);
			resolutionButton->setFontSize(24.0);
			const auto resWidth = AW::StringHelper::getDisplayComponentForDisplayString(resolution, 0);
			const auto resHeight = AW::StringHelper::getDisplayComponentForDisplayString(resolution, 1);
			if (resWidth > maxWindowedWidth || resHeight > maxWindowedHeight)
			{
				continue;
			}

			if (config.mode == AW::ScreenModes::FullscreenDesktop
				|| resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
			{
				resolutionButton->setEnabled(false);
				resolutionButton->setColor(resolutionButtonDisabledColor);
			}

			if (resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight())))
			{
				resolutionButton->setColor(resolutionButtonSelectedColor);
			}
			else if (resolutionButton->getEnabled())
			{
				resolutionButton->setColor(resolutionButtonColor);
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

		wireframeModeCheckbox = std::make_shared<CheckBoxBasic>();
		wireframeModeCheckbox->setText("Wireframe Mode");
		wireframeModeCheckbox->setChecked(config.openGlWireframeMode);
		wireframeModeCheckbox->clickListener = weak_from_this();
		centeringContainer->add(wireframeModeCheckbox);

		debugRenderingCheckbox = std::make_shared<CheckBoxBasic>();
		debugRenderingCheckbox->setText("Debug Rendering");
		debugRenderingCheckbox->setChecked(config.visualizeContainers || config.visualizeClipRects || config.visualizePhysic);
		debugRenderingCheckbox->clickListener = weak_from_this();
		centeringContainer->add(debugRenderingCheckbox);

		debugOverlayCheckbox = std::make_shared<CheckBoxBasic>();
		debugOverlayCheckbox->setText("Debug Overlay");
		debugOverlayCheckbox->setChecked(config.debugOverlayEnabled);
		debugOverlayCheckbox->clickListener = weak_from_this();
		centeringContainer->add(debugOverlayCheckbox);

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
		const auto horScreenPadding = fullscreenDesktopCheckbox->getHalfWidth() + 50.0;

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

		vsyncLabel->toBottomLeftOf(windowedCheckbox, 0.0, checkboxYGroupOffset);
		vsyncOffCheckbox->toBottomRightOf(vsyncLabel, 0.0, checkboxYOffset);
		vsyncOnCheckbox->toRightOf(vsyncOffCheckbox, checkboxYOffset);
		vsyncAdaptiveCheckbox->toRightOf(vsyncOnCheckbox, checkboxYOffset);

		frameLimiterLabelPrefix->toBottomLeftOf(vsyncOffCheckbox, 0.0, checkboxYGroupOffset);
		frameLimiterLabelValue->toRightOf(frameLimiterLabelPrefix);
		frameLimiterScrollBar->toBottomLeftOf(frameLimiterLabelPrefix, 0.0, checkboxYOffset);

		miscLabel->toRightOf(displayModeLabel, fullscreenDesktopCheckbox->getWidth() - displayModeLabel->getWidth() + optionsXOffset + 40);
		wireframeModeCheckbox->toBottomLeftOf(miscLabel, 0.0, checkboxYOffset);
		debugRenderingCheckbox->toBottomLeftOf(wireframeModeCheckbox, 0.0, checkboxYOffset);
		debugOverlayCheckbox->toBottomLeftOf(debugRenderingCheckbox, 0.0, checkboxYOffset);

		soundLabel->toBottomLeftOf(debugOverlayCheckbox, 0.0, checkboxYGroupOffset);
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

		if (id == wireframeModeCheckbox->getId())
		{
			config.openGlWireframeMode = !config.openGlWireframeMode;
			wireframeModeCheckbox->setChecked(config.openGlWireframeMode);
			shouldEnableApply = true;
		}

		if (id == debugRenderingCheckbox->getId())
		{
			config.visualizeClipRects = !(config.visualizeClipRects || config.visualizeContainers || config.visualizePhysic);
			config.visualizeContainers = config.visualizeClipRects;
			config.visualizePhysic = config.visualizeClipRects;
			debugRenderingCheckbox->setChecked(config.visualizeClipRects);
			shouldEnableApply = true;
		}

		if (id == debugOverlayCheckbox->getId())
		{
			config.debugOverlayEnabled = !config.debugOverlayEnabled;
			debugOverlayCheckbox->setChecked(config.debugOverlayEnabled);
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
			const auto resolution = resolutionButton->getText();
			const auto resWidth = AW::StringHelper::getDisplayComponentForDisplayString(resolution, 0);
			const auto resHeight = AW::StringHelper::getDisplayComponentForDisplayString(resolution, 1);
			const auto maxWindowedWidth = modules->screen->getMaxWindowedWidth();
			const auto maxWindowedHeight = modules->screen->getMaxWindowedHeight();
			const auto disabled = config.mode == AW::ScreenModes::FullscreenDesktop
				|| resolution == (std::to_string(getScreenWidth()) + "x" + std::to_string(getScreenHeight()))
				|| resWidth > maxWindowedWidth
				|| resHeight > maxWindowedHeight;

			resolutionButton->setEnabled(!disabled);

			if (resolution == (std::to_string(config.width) + "x" + std::to_string(config.height)))
			{
				resolutionButton->setColor(resolutionButtonSelectedColor);
			}
			else if (disabled)
			{
				resolutionButton->setColor(resolutionButtonDisabledColor);
			}
			else
			{
				resolutionButton->setColor(resolutionButtonColor);
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
			applyButton->setColor(applyButtonColor);
		}

		if (shouldNotifyApplication)
		{
			modules->event->pushEvent(std::make_shared<AW::ReprovisionScreenApplicationEvent>(config));
			applyButton->setEnabled(false);
			applyButton->setColor(applyButtonDisabledColor);
		}
	}

	void SceneOptionsMenu::onKeyPressed(AWKey key)
	{
		switch (key)
		{
		case AWKey::RETURN:
			if (applyButton->getEnabled() == true)
			{
				modules->event->pushEvent(std::make_shared<AW::ReprovisionScreenApplicationEvent>(config));
				applyButton->setEnabled(false);
			}
			break;

		case AWKey::BACKSPACE:
			config = AW::ScreenConfig();
			modules->sound->setMasterVolume(1.0);
			modules->sound->setEffectVolume(0.8);
			modules->sound->setMusicVolume(0.6);
			modules->event->pushEvent(std::make_shared<AW::ReprovisionScreenApplicationEvent>(config));
			applyButton->setEnabled(false);
			applyButton->setColor(applyButtonDisabledColor);
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
			applyButton->setColor(applyButtonColor);
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

		config.width = getScreenWidth();
		config.height = getScreenHeight();

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
		if (config.frameLimiter < frameLimits[1])
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