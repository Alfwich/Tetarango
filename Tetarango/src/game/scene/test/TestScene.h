#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/scene/Scene.h"
#include "engine/module/event/EnterFrameListener.h"
#include "gui/button/ButtonBasic.h"
#include "gui/IGuiListener.h"
#include "ui/scene/SceneTransitionBundle.h"
#include "gui/camera/GameCamera.h"
#include "gui/IGuiListener.h"
#include "gui/scrollbar/ScrollBarBasic.h"
#include "ui/camera/ICameraListener.h"

namespace AWGame
{

	class TestScene : public AW::Scene, public AW::ICameraListener, public IGuiListener
	{
		double currentIters = 15;
		bool itersIncPressed = false, itersDecPressed = false;
		std::shared_ptr<GameCamera> camera;

		std::shared_ptr<ScrollBarBasic> red, green, blue;
		std::shared_ptr<AW::Text> infoLabel;
		std::shared_ptr<Renderable> obj1, obj2, obj3, obj4;

		std::shared_ptr<AW::Timer> iterTimer, updateTimer;

	public:
		TestScene();

		void onLoadResources();

		void onInitialAttach();
		void onAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(const double& deltaTime);
		void onKeyPressed(SDL_Scancode key);
		void onKey(SDL_Scancode key, bool isPressed);

		void onScrollBarScroll(int id, double pos);
		void onCameraUpdate();
	};

}
