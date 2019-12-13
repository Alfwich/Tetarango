#pragma once

#include "ui/renderable/element/Text.h"
#include "gui/button/ButtonBasic.h"
#include "gui/button/CheckBoxBasic.h"
#include "gui/scrollbar/ScrollBarBasic.h"
#include "scene/BaseScene.h"
#include "ui/renderable/container/ScrollContainer.h"
#include "gui/scrollarea/ScrollArea.h"

namespace MTGame
{

	class SceneOptionsMenu : public BaseScene
	{
		MT::DisplayModeInfo info;
		MT::ScreenConfig config;
		std::shared_ptr<MT::Text> optionsMenuTitle;
		std::shared_ptr<ButtonBasic> backButton;
		std::shared_ptr<ButtonBasic> applyButton;
		std::shared_ptr<ButtonBasic> resetButton;
		std::shared_ptr<CheckBoxBasic> windowedCheckbox;
		std::shared_ptr<CheckBoxBasic> fullscreenCheckbox;
		std::shared_ptr<CheckBoxBasic> fullscreenDesktopCheckbox;
		std::shared_ptr<CheckBoxBasic> msaaOffCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa2xCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa4xCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa8xCheckbox;
		std::shared_ptr<CheckBoxBasic> openGlCompatibilityModeCheckbox;
		std::shared_ptr<CheckBoxBasic> wireframeModeCheckbox;

		std::shared_ptr<ScrollArea> resolutionScrollArea;
		std::vector<std::shared_ptr<ButtonBasic>> resolutionButtons;

		bool setMsaaMode(int samples);
		bool setScreenMode(MT::ScreenModes mode);

	public:
		SceneOptionsMenu();

		void onInitialAttach();
		void onAttach();
		void onDestroyChildren();
		void onCreateChildren();
		void onLayoutChildren();

		void onButtonClicked(int id);
		void onKeyPressed(SDL_Scancode key);
	};

}