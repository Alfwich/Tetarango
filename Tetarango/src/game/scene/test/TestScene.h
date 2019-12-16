#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/scene/Scene.h"
#include "engine/module/event/EnterFrameListener.h"
#include "gui/button/ButtonBasic.h"
#include "gui/IGuiListener.h"
#include "ui/scene/SceneTransitionBundle.h"
#include "gui/camera/GameCamera.h"

namespace AWGame
{

	class TestScene : public AWCore::Scene
	{
		std::shared_ptr<GameCamera> camera;

	public:
		TestScene();

		void onInitialAttach();
		void onAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(double deltaTime);
		void onKeyPressed(SDL_Scancode key);
		void onMouseButtonLeftDown();
	};

}
