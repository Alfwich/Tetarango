#include "TestScene2.h"

#include <random>
#include "GameImports.h"
#include "ui/renderable/element/DisplayBuffer.h"
#include "generator/block/GeneratorBlock.h"

namespace
{
	const char* backButtonId = "back_button";
	const std::string player = "player";
	AWGame::GeneratorBlock blockColorGenerator;
	const auto spawnMs = 1;
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
	}

	void TestScene2::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
		modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		modules->physic->setWorldFps(0, 60);

		modules->physic->registerWorld(1);
		modules->physic->setWorldFps(1, 20);
	}

	void TestScene2::onCreateChildren()
	{
		contentContainer = std::make_shared<AW::Container>();
		contentContainer->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		contentContainer->topLeftAlignSelf();
		add(contentContainer);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(16000000.0, 0);
		camera->setDefaultZoomAndAnchorPoint(1.0, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		contentContainer->add(camera);

		const auto b = std::make_shared<sBlock>();
		modules->physic->registerRigidBodyForWorld(0, b);
		contentContainer->add(b);
	}

	void TestScene2::onLayoutChildren()
	{
	}

	void TestScene2::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene2::onTimeoutCalled()
	{
		if (isAttached())
		{
			const auto b = std::make_shared<dBlock>();

			if (true || AW::NumberHelper::chance(50))
			{
				modules->physic->registerRigidBodyForWorld(0, b);
				b->setColor(blockColorGenerator.getBlockColor());
			}
			else
			{
				modules->physic->registerRigidBodyForWorld(1, b);
				b->setColor(255, 255, 255);
				b->zIndex = -1;
			}

			contentContainer->add(b);
			modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		}
	}

	void TestScene2::onEnterFrame(const double& deltaTime)
	{
	}

	void TestScene2::onKeyPressed(SDL_Scancode key)
	{
		if (key == SDL_SCANCODE_ESCAPE)
		{
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
		}

		if (key == SDL_SCANCODE_1)
		{
			contentContainer->destroyChildren();
		}
	}

	void TestScene2::onKey(SDL_Scancode key, bool isPressed)
	{
	}

	void TestScene2::onScrollBarScroll(int id, double pos)
	{
	}

	void TestScene2::onCameraUpdate()
	{
	}
}
