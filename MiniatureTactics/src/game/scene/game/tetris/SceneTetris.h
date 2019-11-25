#pragma once

#include "ui/renderable/Container.h"
#include "ui/scene/Scene.h"
#include "engine/module/event/EnterFrameListener.h"
#include "gui/button/ButtonBasic.h"
#include "gui/button/IButtonListener.h"
#include "ui/scene/SceneTransitionBundle.h"
#include "gui/camera/GameCamera.h"
#include "scene/BaseScene.h"

namespace MTGame
{

	class SceneTetris : public BaseScene
	{

	public:
		SceneTetris();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(double deltaTime);
		void onKeyPressed(SDL_Scancode key);
	};
}
