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
		AWCore::DisplayModeInfo info;
		AWCore::ScreenConfig config;
		int stopSoundTimeoutId = 0, playSoundTimeoutId = 0;

		std::shared_ptr<AWCore::Text> optionsMenuTitle;
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
		std::shared_ptr<CheckBoxBasic> openGlCompatibilityModeCheckbox;
		std::shared_ptr<CheckBoxBasic> wireframeModeCheckbox;

		std::shared_ptr<ScrollArea> resolutionScrollArea;
		std::vector<std::shared_ptr<ButtonBasic>> resolutionButtons;

		std::shared_ptr<ScrollBarBasic> masterVolScrollBar;
		std::shared_ptr<ScrollBarBasic> generalVolScrollBar;
		std::shared_ptr<ScrollBarBasic> musicVolScrollBar;

		std::shared_ptr<AWCore::Text> masterVolLabel;
		std::shared_ptr<AWCore::Text> generalVolLabel;
		std::shared_ptr<AWCore::Text> musicVolLabel;

		bool setMsaaMode(int samples);
		bool setScreenMode(AWCore::ScreenModes mode);
		void setVolLabels();
		std::string volumeToString(double volume);

	public:
		SceneOptionsMenu();

		void onInitialAttach();
		void onAttach();
		void onDetach();
		void onDestroyChildren();
		void onCreateChildren();
		void onLayoutChildren();

		void onTimeoutCalled(int id);
		void onButtonClicked(int id);
		void onKeyPressed(SDL_Scancode key);
		void onScrollBarScroll(int id, double position);
	};

}