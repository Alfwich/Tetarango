#include "TestScene2.h"

#include <random>
#include "GameImports.h"
#include "ui/renderable/element/DisplayBuffer.h"
#include "generator/block/GeneratorBlock.h"
#include "prop/physic/Poly.h"

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
		setTimeScope(AW::TimeScope::Game);
		//setShouldRebuildOnLoad();
		GORegister(TestScene2);
	}

	void TestScene2::onLoadResources()
	{
	}

	void TestScene2::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(AW::TimeScope::Game);

		modules->input->keyboard->registerKeys({
				AWKey::ESCAPE,
				AWKey::ONE,
				AWKey::TWO,
				AWKey::THREE,
				AWKey::FOUR,
				AWKey::FIVE,
				AWKey::SIX,
				AWKey::SEVEN,
				AWKey::EIGHT,
				AWKey::UP,
				AWKey::DOWN,
				AWKey::LEFT,
				AWKey::RIGHT
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AWMouseButton::Left, weak_from_this());

		tran = modules->animation->createGameTransition();
	}

	void TestScene2::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
		modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		modules->physic->setWorldFps(0, 60);
		modules->physic->setWorldGravity(0);
		modules->physic->setWorldTimescope(0, AW::TimeScope::Game);
	}

	void TestScene2::onCreateChildren()
	{
		contentContainer = std::make_shared<AW::Container>();
		contentContainer->name = "cc";
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
			platform->setSize(80000.0, 100.0);
			platform->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() - 150);
			contentContainer->add(platform);
		}

		const auto xOff = 700.0;
		const auto yOff = -250.0;

		const auto platformA = std::make_shared<Box>();
		platformA->setDynamic(false);
		platformA->setColor(64, 64, 64);
		platformA->setSize(500.0, 100.0);
		platformA->setRotation(45.0);
		platformA->setPosition(modules->screen->getWidth() / 2.0 - xOff, modules->screen->getHeight() / 2.0 + yOff);
		contentContainer->add(platformA);


		const auto platformB = std::make_shared<Box>();
		platformB->setDynamic(false);
		platformB->setColor(64, 64, 64);
		platformB->setSize(500.0, 100.0);
		platformB->setRotation(-45.0);
		platformB->setPosition(modules->screen->getWidth() / 2.0 + xOff, modules->screen->getHeight() / 2.0 + yOff);
		contentContainer->add(platformB);

		player = std::make_shared<Player>();
		player->name = "player";
		player->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 250.0);
		contentContainer->add(player);

		{
			for (auto i = 0; i < 10; ++i)
			{
				const auto binder = std::make_shared<Box>();
				binder->setDynamic(true);
				binder->setColor(128, 128, 128);
				binder->setSize(50.0, 50.0);
				binder->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 250.0);
				contentContainer->add(binder);

				const auto joint = std::make_shared<AW::Joint>();
				joint->setJointType(AW::JointType::Distant);
				if (AW::NumberHelper::chance(50))
				{
					joint->bodyA = platformA;
				}
				else
				{
					joint->bodyA = platformB;
				}

				joint->bodyB = binder;
				joint->setJointDistance(AW::NumberHelper::random(50, 200));
				contentContainer->add(joint);
			}
		}

		camera->target = player;

		const auto dim = 100.0;
		for (auto i = 0; i < 50; ++i)
		{
			const auto poly = std::make_shared<Poly>();
			poly->setColor(AW::Color::random());

			const auto pts = AW::NumberHelper::randomInt(3, 8);
			const auto d = (AW::NumberHelper::PI * 2.0) / pts;
			for (auto i = 0; i < pts; ++i)
			{
				const auto x = std::cos(i * d) * dim;
				const auto y = std::sin(i * d) * dim;
				poly->setPoint(x, y);
			}
			poly->setRotation(AW::NumberHelper::random(360));

			poly->setPosition((modules->screen->getWidth() / 2.0) + AW::NumberHelper::random(-3000.0, 3000.0), AW::NumberHelper::random(modules->screen->getHeight() / 2.0));
			contentContainer->add(poly);
		}
	}

	void TestScene2::onLayoutChildren()
	{
	}

	void TestScene2::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
		player = findChildWithName<Player>("player");
		camera->target = player;
		contentContainer = findChildWithName<AW::Container>("cc");
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
		if (contentContainer == nullptr) rebuild();

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

	void TestScene2::onKeyPressed(AWKey key)
	{
		if (key == AWKey::ESCAPE)
		{
			destroyChildren();
			camera = nullptr;
			player = nullptr;
			contentContainer = nullptr;
			obj1 = obj2 = nullptr;
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::SavedGamesMenu));
		}

		if (key == AWKey::ONE)
		{
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
		}

		if (key == AWKey::TWO)
		{
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 2.0);
		}

		if (key == AWKey::THREE)
		{
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 0.5);
		}

		if (key == AWKey::FOUR)
		{
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 0.1);
		}

		if (key == AWKey::FIVE)
		{
			modules->physic->setWorldGravity(0, 0.0, 0.0);
		}

		if (key == AWKey::FIVE)
		{
			modules->physic->setWorldGravity(0);
		}
	}

	void TestScene2::onKey(AWKey key, bool isPressed)
	{
	}

	void TestScene2::onMouseButton(AWMouseButton button, bool isPressed)
	{
	}

	void TestScene2::onScrollBarScroll(int id, double pos)
	{
	}

	void TestScene2::onCameraUpdate()
	{
	}
}
