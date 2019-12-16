#pragma once

#include "ui/scene/Scene.h"
#include "engine/module/event/EnterFrameListener.h"
#include "gui/camera/GameCamera.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"

namespace AWGame
{

	class TestSpace2 : public AWCore::Scene
	{
		std::shared_ptr<GameCamera> camera;
		std::shared_ptr<AWCore::ParticleSystem> globalSystem;

	public:
		TestSpace2();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onAttach();
		void onEnterFrame(double frameTime);

		void onKeyPressed(SDL_Scancode key);
	};

}

