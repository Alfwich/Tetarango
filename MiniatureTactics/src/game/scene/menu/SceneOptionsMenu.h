#pragma once

#include "ui/renderable/element/Text.h"
#include "gui/button/ButtonBasic.h"
#include "scene/BaseScene.h"

namespace MTGame
{

	class SceneOptionsMenu : public BaseScene
	{
		std::shared_ptr<MT::Text> optionsMenuTitle;
		std::shared_ptr<ButtonBasic> backButton;

	public:
		SceneOptionsMenu();

		void onCreateChildren();
		void onChildrenHydrated();

		void onButtonClicked(int id);
	};

}