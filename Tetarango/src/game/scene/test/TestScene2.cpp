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
				SDL_SCANCODE_UP,
				SDL_SCANCODE_DOWN,
				SDL_SCANCODE_LEFT,
				SDL_SCANCODE_RIGHT
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());
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
			const auto b = std::make_shared<sBlock>();
			b->setColor(64, 64, 64);
			b->setSize(600.0, 100.0);
			b->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
			modules->physic->registerRigidBodyForWorld(0, b);
			contentContainer->add(b);
		}

		const auto xOff = 700.0;
		const auto yOff = 0.0;
		{
			const auto b = std::make_shared<sBlock>();
			b->setColor(64, 64, 64);
			b->setSize(500.0, 100.0);
			b->setRotation(45.0);
			b->setPosition(modules->screen->getWidth() / 2.0 - xOff, modules->screen->getHeight() / 2.0 - yOff);
			modules->physic->registerRigidBodyForWorld(0, b);
			contentContainer->add(b);
		}

		{
			const auto b = std::make_shared<sBlock>();
			b->setColor(64, 64, 64);
			b->setSize(500.0, 100.0);
			b->setRotation(-45.0);
			b->setPosition(modules->screen->getWidth() / 2.0 + xOff, modules->screen->getHeight() / 2.0 - yOff);
			modules->physic->registerRigidBodyForWorld(0, b);
			contentContainer->add(b);
		}

		{
			follower = std::make_shared<dBlock>();
			follower->setColor(255, 255, 255);
			follower->setSize(200, 200);
			follower->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 230.0);
			modules->physic->registerRigidBodyForWorld(0, follower);
			//follower->setFixedRotation(true);
			follower->setMass(50.0);
			contentContainer->add(follower);
		}

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
				const auto b = std::make_shared<dBlock>();
				b->setAlpha(0.0);
				b->setSize(32.0, 32.0);
				b->setPosition(modules->screen->getWidth() / 2.0 + AW::NumberHelper::random(-400.0, 400.0), modules->screen->getHeight() / 2.0 + AW::NumberHelper::random(-500.0, -1000.0));
				b->setColor(blockColorGenerator.getBlockColor());

				modules->physic->registerRigidBodyForWorld(0, b);

				contentContainer->add(b);
			}

			modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		}
	}

	void TestScene2::onEnterFrame(const double& deltaTime)
	{
		if (followButtonPressed)
		{
			const auto x1 = follower->getWorldRect()->x, y1 = follower->getWorldRect()->y, x2 = (double)modules->input->mouse->X(), y2 = (double)modules->input->mouse->Y();
			const auto v = std::atan2(y1 - y2, x1 - x2);
			const auto vX = x2 - x1;
			const auto vY = y1 - y2;
			const auto mag = std::sqrt(vX * vX + vY * vY);
			follower->applyForce(vX / mag, vY / mag, 160000.0 * (deltaTime / 1000.0));
		}

		if (upPressed) follower->applyForce(0.0, 1.0, 160000.0 * (deltaTime / 1000.0));
		else if (downPressed) follower->applyForce(0.0, -1.0, 160000.0 * (deltaTime / 1000.0));
		else if (leftPressed) follower->applyForce(-1.0, 0.0, 160000.0 * (deltaTime / 1000.0));
		else if (rightPressed) follower->applyForce(1.0, 0.0, 160000.0 * (deltaTime / 1000.0));
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
		if (key == SDL_SCANCODE_SPACE) followButtonPressed = isPressed;

		if (key == SDL_SCANCODE_UP) upPressed = isPressed;
		if (key == SDL_SCANCODE_DOWN) downPressed = isPressed;
		if (key == SDL_SCANCODE_LEFT) leftPressed = isPressed;
		if (key == SDL_SCANCODE_RIGHT) rightPressed = isPressed;
	}

	void TestScene2::onMouseButton(AW::MouseButton button, bool isPressed)
	{
		if (button == AW::MouseButton::Left) followButtonPressed = isPressed;
	}

	void TestScene2::onScrollBarScroll(int id, double pos)
	{
	}

	void TestScene2::onCameraUpdate()
	{
	}
}
