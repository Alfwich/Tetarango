#include "TestScene2.h"

#include <random>
#include "GameImports.h"
#include "ui/renderable/element/DisplayBuffer.h"

namespace
{
	const char* backButtonId = "back_button";
	const std::string player = "player";
}

namespace AWGame
{

	TestScene2::TestScene2() : AW::Scene("test_scene_2")
	{
		setShouldRebuildOnLoad();
		registerGameObject<TestScene2>();
	}

	void TestScene2::onLoadResources()
	{
	}

	void TestScene2::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(AW::TimeScope::Game);

		modules->input->keyboard->registerKeys({
				SDL_SCANCODE_ESCAPE,
				SDL_SCANCODE_1,
				SDL_SCANCODE_2,
				SDL_SCANCODE_3,
				SDL_SCANCODE_LEFT,
				SDL_SCANCODE_RIGHT,
				SDL_SCANCODE_UP,
				SDL_SCANCODE_LEFTBRACKET,
				SDL_SCANCODE_RIGHTBRACKET,
				SDL_SCANCODE_UP,
				SDL_SCANCODE_DOWN
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());

		iterTimer = modules->time->createTimer();
		iterTimer->start();

		updateTimer = modules->time->createTimer();
		updateTimer->start();
	}

	void TestScene2::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
	}

	void TestScene2::onCreateChildren()
	{
		const auto contentContainer = std::make_shared<AW::Rectangle>();
		contentContainer->renderColorMode = AW::RenderColorMode::Absolute;
		contentContainer->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		contentContainer->topLeftAlignSelf();
		add(contentContainer);
		this->contentContainer = contentContainer;

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(16000000.0, 0);
		camera->setDefaultZoomAndAnchorPoint(1.0, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		contentContainer->add(camera);
	}

	void TestScene2::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene2::onEnterFrame(const double& deltaTime)
	{
		if ((itersIncPressed || itersDecPressed) && iterTimer->isAboveThresholdAndRestart(10))
		{
			if (itersIncPressed)
			{
				currentIters = AW::NumberHelper::clamp(currentIters + 1.0, 0.0, 1000.0);
				obj1->getFragmentShader()->setFloatIUParam("iter", currentIters);
				infoLabel->setText("Iters: " + std::to_string((int)currentIters));
				obj2->markDirty();
			}
			else if (itersDecPressed)
			{
				currentIters = AW::NumberHelper::clamp(currentIters - 1.0, 0.0, 1000.0);
				obj1->getFragmentShader()->setFloatIUParam("iter", currentIters);
				infoLabel->setText("Iters: " + std::to_string((int)currentIters));
				obj2->markDirty();
			}
		}

		contentContainer->rotate((deltaTime / 1000.0) * 15.0);
	}

	void TestScene2::onKeyPressed(SDL_Scancode key)
	{
		if (key == SDL_SCANCODE_ESCAPE)
		{
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
		}
	}

	void TestScene2::onKey(SDL_Scancode key, bool isPressed)
	{
		if (key == SDL_SCANCODE_1)
		{
			itersDecPressed = isPressed;
		}

		if (key == SDL_SCANCODE_2)
		{
			itersIncPressed = isPressed;
		}

		if (key == SDL_SCANCODE_3 && isPressed)
		{
			obj2->renderMode = obj2->renderMode == AW::RenderMode::CachedElement ? AW::RenderMode::ChildrenOnly : AW::RenderMode::CachedElement;
			obj2->markDirty();
		}
	}

	void TestScene2::onScrollBarScroll(int id, double pos)
	{
	}

	void TestScene2::onCameraUpdate()
	{
	}
}
