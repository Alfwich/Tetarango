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

	class TestScene : public AW::Scene, public AW::ICameraListener, public AW::INetworkListener, public IGuiListener
	{
		double currentIters = 15, layoutMotionTime = 0.0;
		bool itersIncPressed = false, itersDecPressed = false;
		std::shared_ptr<GameCamera> camera;

		std::shared_ptr<ScrollBarBasic> red, green, blue, xF, yF;
		std::shared_ptr<AW::Text> infoLabel;
		std::shared_ptr<Renderable> obj1, obj2, obj3, obj4, contentContainer;

		std::vector<std::shared_ptr<Renderable>> layoutExampleRects;

		std::shared_ptr<AW::Timer> iterTimer, updateTimer;

	public:
		TestScene();

		void onLoadResources();

		void onInitialAttach();
		void onAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(const double& deltaTime);
		void onKeyPressed(AWKey key);
		void onKey(AWKey key, bool isPressed);

		void onScrollBarScroll(int id, double pos);

		void onCameraUpdate();
		void onNetworkCallCompleted(int status, std::string& body) override;
	};

}
