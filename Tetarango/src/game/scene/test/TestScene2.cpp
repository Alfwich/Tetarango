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
	const auto spawnMs = 25;
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
				SDL_SCANCODE_4,
				SDL_SCANCODE_5,
				SDL_SCANCODE_6,
				SDL_SCANCODE_7,
				SDL_SCANCODE_8,
				SDL_SCANCODE_UP,
				SDL_SCANCODE_DOWN,
				SDL_SCANCODE_LEFT,
				SDL_SCANCODE_RIGHT
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());

		tran = modules->animation->createGameTransition();
	}

	void TestScene2::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
		modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		modules->physic->setWorldFps(0, 60);
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

		{
			const auto platform = std::make_shared<Box>();
			platform->setDynamic(false);
			platform->setColor(64, 64, 64);
			platform->setSize(600.0, 100.0);
			platform->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
			contentContainer->add(platform);
		}

		const auto xOff = 700.0;
		const auto yOff = -250.0;
		{
			const auto platform = std::make_shared<Box>();
			platform->setDynamic(false);
			platform->setColor(64, 64, 64);
			platform->setSize(500.0, 100.0);
			platform->setRotation(45.0);
			platform->setPosition(modules->screen->getWidth() / 2.0 - xOff, modules->screen->getHeight() / 2.0 + yOff);
			contentContainer->add(platform);
		}

		{
			const auto platform = std::make_shared<Box>();
			platform->setDynamic(false);
			platform->setColor(64, 64, 64);
			platform->setSize(500.0, 100.0);
			platform->setRotation(-45.0);
			platform->setPosition(modules->screen->getWidth() / 2.0 + xOff, modules->screen->getHeight() / 2.0 + yOff);
			contentContainer->add(platform);
		}

		player = std::make_shared<Player>();
		player->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 250.0);
		contentContainer->add(player);
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
			if (contentContainer->getChildren().size() < 400)
			{
				const auto b = std::make_shared<Box>();
				b->setAlpha(0.0);

				b->setDynamic(true);
				b->setSize(50, 50);
				b->setPosition(modules->screen->getWidth() / 2.0 + AW::NumberHelper::random(-400.0, 400.0), modules->screen->getHeight() / 2.0 + AW::NumberHelper::random(-800.0, -1000.0));
				b->setColor(blockColorGenerator.getBlockColor());

				contentContainer->add(b);
			}

			modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		}
	}

	void TestScene2::onEnterFrame(const double& deltaTime)
	{
		for (const auto c : contentContainer->getChildrenOfType<Box>())
		{
			if (c->getY() > 4000)
			{
				if (c->getAlpha() <= 0.0)
				{
					c->removeFromParent();
				}
				else
				{
					c->setAlpha(c->getAlpha() - deltaTime / 1000.0);
				}
			}
			else
			{
				c->setAlpha(c->getAlpha() + deltaTime / 1000.0);
			}
		}
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
		}

		if (key == SDL_SCANCODE_2)
		{
			tran->stop();
		}

		if (key == SDL_SCANCODE_3)
		{
			modules->physic->setWorldGravity(0, 0.0, 0.0);
		}

		if (key == SDL_SCANCODE_4)
		{
			modules->physic->setWorldGravity(0);
		}

		if (key == SDL_SCANCODE_5)
		{
		}

	}

	void TestScene2::onKey(SDL_Scancode key, bool isPressed)
	{
	}

	void TestScene2::onMouseButton(AW::MouseButton button, bool isPressed)
	{
	}

	void TestScene2::onScrollBarScroll(int id, double pos)
	{
	}

	void TestScene2::onCameraUpdate()
	{
	}
}
