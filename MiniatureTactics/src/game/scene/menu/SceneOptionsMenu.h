#pragma once

#include "ui/renderable/element/Text.h"
#include "gui/button/ButtonBasic.h"
#include "gui/button/CheckBoxBasic.h"
#include "gui/scrollbar/ScrollBarBasic.h"
#include "scene/BaseScene.h"
#include "ui/renderable/container/ScrollContainer.h"

namespace MTGame
{

	class SceneOptionsMenu : public BaseScene
	{
		MT::DisplayModeInfo info;
		MT::ScreenConfig config;
		std::shared_ptr<MT::Text> optionsMenuTitle;
		std::shared_ptr<ButtonBasic> backButton;
		std::shared_ptr<CheckBoxBasic> windowedCheckbox;
		std::shared_ptr<CheckBoxBasic> fullscreenCheckbox;
		std::shared_ptr<CheckBoxBasic> fullscreenDesktopCheckbox;
		std::shared_ptr<CheckBoxBasic> msaaOffCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa2xCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa4xCheckbox;
		std::shared_ptr<CheckBoxBasic> msaa8xCheckbox;
		std::shared_ptr<CheckBoxBasic> openGlCompatibilityModeCheckbox;
		std::shared_ptr<CheckBoxBasic> wireframeModeCheckbox;
		
		std::shared_ptr<MT::ScrollContainer> scrollContainer;
		std::shared_ptr<ScrollBarBasic> scrollContainerScrollBar;
		std::vector<std::shared_ptr<ButtonBasic>> resolutionButtons;

		bool testState = false;

	public:
		SceneOptionsMenu();

		void onInitialAttach();
		void onDestroyChildren();
		void onCreateChildren();

		void onEnterFrame(double frameTime);

		void onButtonClicked(int id);
	};

}