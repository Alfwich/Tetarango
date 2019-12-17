#pragma once

#include "ui/renderable/element/Text.h"
#include "gui/button/ButtonBasic.h"
#include "gui/button/CheckBoxBasic.h"
#include "gui/scrollbar/ScrollBarBasic.h"
#include "scene/BaseScene.h"
#include "ui/renderable/container/ScrollContainer.h"
#include "gui/scrollarea/ScrollArea.h"

namespace AWGame
{

	class SceneOptionsMenu : public BaseScene
	{
		AW::DisplayModeInfo info;
		AW::ScreenConfig config;
		int stopSoundTimeoutId = 0, playSoundTimeoutId = 0;

		std::shared_ptr<AW::Container> rootContainer;
		std::shared_ptr<AW::Container> centeringContainer;

		std::shared_ptr<AW::Text> optionsMenuTitle;
		std::shared_ptr<ButtonBasic> backButton;
		std::shared_ptr<ButtonBasic> applyButton;
		std::shared_ptr<ButtonBasic> resetButton;

		std::shared_ptr<CheckBoxBasic> windowedCheckbox;
		std::shared_ptr<CheckBoxBasic> fullscreenCheckbox;
		std::shared_ptr<CheckBoxBasic> fullscreenDesktopCheckbox;

		std::shared_ptr<CheckBoxBasic> msaaOffCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa4xCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa8xCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa16xCheckbox;

		std::shared_ptr<CheckBoxBasic> vsyncOffCheckbox;
		std::shared_ptr<CheckBoxBasic> vsyncOnCheckbox;
		std::shared_ptr<CheckBoxBasic> vsyncAdaptiveCheckbox;

		std::shared_ptr<CheckBoxBasic> openGlCompatibilityModeCheckbox;
		std::shared_ptr<CheckBoxBasic> wireframeModeCheckbox;
		std::shared_ptr<CheckBoxBasic> debugRenderingCheckbox;

		std::shared_ptr<ScrollArea> resolutionScrollArea;
		std::vector<std::shared_ptr<ButtonBasic>> resolutionButtons;

		std::shared_ptr<ScrollBarBasic> masterVolScrollBar;
		std::shared_ptr<ScrollBarBasic> generalVolScrollBar;
		std::shared_ptr<ScrollBarBasic> musicVolScrollBar;
		std::shared_ptr<ScrollBarBasic> frameLimiterScrollBar;

		std::shared_ptr<AW::Text> resolutionsLabel;
		std::shared_ptr<AW::Text> displayModeLabel;
		std::shared_ptr<AW::Text> vsyncLabel;
		std::shared_ptr<AW::Text> msaaLabel;
		std::shared_ptr<AW::Text> miscLabel;
		std::shared_ptr<AW::Text> soundLabel;
		std::shared_ptr<AW::Text> masterVolLabel;
		std::shared_ptr<AW::Text> generalVolLabel;
		std::shared_ptr<AW::Text> musicVolLabel;
		std::shared_ptr<AW::Text> frameLimiterLabelPrefix;
		std::shared_ptr<AW::Text> frameLimiterLabelValue;

		bool setMsaaMode(int samples);
		bool setScreenMode(AW::ScreenModes mode);
		bool setVsyncMode(AW::VsyncModes mode);
		void setDynamicLabels();
		std::string volumeToString(double volume);
		int positionToFrameLimit(double position);
		double frameLimitToPosition(int limit);

	public:
		SceneOptionsMenu();

		void onInitialAttach();
		void onDetach();
		void onDestroyChildren();
		void onCreateChildren();
		void onLayoutChildren();

		void onTransitionedTo();

		void onTimeoutCalled(int id);
		void onButtonClicked(int id);
		void onKeyPressed(SDL_Scancode key);
		void onScrollBarScroll(int id, double position);
	};

}