#pragma once

#include "ui/scene/Scene.h"
#include "gui/camera/GameCamera.h"
#include "gui/IGuiListener.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Rectangle.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/physic/Box.h"
#include "actor/player/Player.h"
#include "ui/physic/joint/Joint.h"

namespace AWGame
{
	class TestScene2 : public AW::Scene, public AW::ICameraListener, public IGuiListener
	{
		std::shared_ptr<GameCamera> camera;
		std::shared_ptr<Player> player;
		std::shared_ptr<AW::Container> contentContainer;
		std::shared_ptr<AW::Renderable> obj1, obj2;
		std::shared_ptr<AW::Transition> tran;

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
		void onKeyPressed(AWKey key);
		void onKey(AWKey key, bool isPressed);
		void onMouseButton(AWMouseButton button, bool isPressed);

		void onScrollBarScroll(int id, double pos);
		void onCameraUpdate();
	};

}
