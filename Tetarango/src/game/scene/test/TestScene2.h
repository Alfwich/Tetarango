#pragma once

#include <vector>
#include "ui/scene/Scene.h"
#include "gui/camera/GameCamera.h"
#include "gui/IGuiListener.h"
#include "box2d/b2_world.h"
#include "box2d/box2d.h"
#include "ui/renderable/primitive/Rectangle.h"

namespace AWGame
{

	class TestScene2 : public AW::Scene, public AW::ICameraListener, public IGuiListener
	{

		std::shared_ptr<GameCamera> camera;

		b2World* world;
		b2Body* groundBody;

		std::shared_ptr<Renderable> obj1, obj2;
		std::vector<std::pair<b2Body*, std::shared_ptr<AW::Renderable>>> objs;

	public:
		TestScene2();

		void onLoadResources();

		void onInitialAttach();
		void onAttach();
		void onCreateChildren();
		void onLayoutChildren();
		void onChildrenHydrated();
		void onTimeoutCalled();

		void onEnterFrame(const double& deltaTime);
		void onKeyPressed(SDL_Scancode key);
		void onKey(SDL_Scancode key, bool isPressed);

		void onScrollBarScroll(int id, double pos);
		void onCameraUpdate();
	};

}
