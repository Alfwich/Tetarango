#pragma once

#include "ui/renderable/element/Text.h"
#include "gui/button/ButtonBasic.h"
#include "gui/button/CheckBoxBasic.h"
#include "scene/BaseScene.h"
#include "ui/renderable/container/ScrollContainer.h"

namespace MTGame
{

	class SceneOptionsMenu : public BaseScene
	{
		MT::DisplayModeInfo info;
		std::shared_ptr<MT::Text> optionsMenuTitle;
		std::shared_ptr<ButtonBasic> backButton;
		std::shared_ptr<CheckBoxBasic> fullscreenCheckbox;
		std::shared_ptr<MT::ScrollContainer> scrollContainer;
		std::vector<std::shared_ptr<ButtonBasic>> resolutionButtons;

	public:
		SceneOptionsMenu();

		void onInitialAttach();
		void onDestroyChildren();
		void onCreateChildren();

		void onButtonClicked(int id);
	};

}