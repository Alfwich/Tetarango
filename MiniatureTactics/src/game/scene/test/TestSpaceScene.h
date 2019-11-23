#pragma once

#include "ui/scene/Scene.h"
#include "engine/module/event/EnterFrameListener.h"
#include "gui/camera/GameCamera.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"

namespace MTGame
{

	class TestSpaceScene : public MT::Scene
	{
		std::shared_ptr<GameCamera> camera;
		std::shared_ptr<MT::ParticleSystem> globalSystem;

	public:
		TestSpaceScene();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onAttach();
		void onEnterFrame(double frameTime);

		void onKeyPressed(SDL_Scancode key);
	};

}

