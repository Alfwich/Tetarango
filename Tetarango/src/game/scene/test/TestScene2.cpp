#include "TestScene2.h"

#include "scene/BaseScene.h"
#include "ui/renderable/element/DisplayBuffer.h"
#include "generator/block/GeneratorBlock.h"
#include "ui/renderable/physic/Poly.h"
#include "ui/renderable/physic/Chain.h"
#include "scene/BaseScene.h"

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
		setShouldRebuildOnLoad();
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
		contentContainer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		contentContainer->topLeftAlignSelf();
		add(contentContainer);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(16000000.0, 0);
		camera->setDefaultZoomAndAnchorPoint(1.0, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		contentContainer->add(camera);

		const auto screenWidthInMeters = AW::Renderable::screenToWorldPosition(modules->screen->getWidth());
		const auto screenHeightInMeters = AW::Renderable::screenToWorldPosition(modules->screen->getHeight());
		{
			const auto platform = std::make_shared<AW::Box>();
			platform->setDynamic(false);
			platform->setColor(64, 64, 64);
			platform->setWorldSize(20.0, 0.5);
			platform->setWorldPosition(screenWidthInMeters / 2.0, -(screenHeightInMeters / 2.0) - 5.0);
			contentContainer->add(platform);
		}

		const auto xOff = 700.0;
		const auto yOff = -250.0;

		const auto platformA = std::make_shared<AW::Box>();
		platformA->setDynamic(false);
		platformA->setColor(64, 64, 64);
		platformA->setScreenSize(500.0, 100.0);
		platformA->setScreenRotation(45.0);
		platformA->setScreenPosition(modules->screen->getWidth() / 2.0 - xOff, modules->screen->getHeight() / 2.0 + yOff);
		contentContainer->add(platformA);


		const auto platformB = std::make_shared<AW::Box>();
		platformB->setDynamic(false);
		platformB->setColor(64, 64, 64);
		platformB->setScreenSize(500.0, 100.0);
		platformB->setScreenRotation(-45.0);
		platformB->setScreenPosition(modules->screen->getWidth() / 2.0 + xOff, modules->screen->getHeight() / 2.0 + yOff);
		contentContainer->add(platformB);

		player = std::make_shared<Player>();
		player->name = "player";
		player->setScreenPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 250.0);
		contentContainer->add(player);

		{
			for (auto i = 0; i < 10; ++i)
			{
				const auto binder = std::make_shared<AW::Box>();
				binder->setDynamic(true);
				binder->setColor(128, 128, 128);
				binder->setScreenSize(50.0, 50.0);
				binder->setScreenPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 250.0);
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
		for (auto i = 0; i < 10; ++i)
		{
			const auto poly = std::make_shared<AW::Poly>();
			poly->setColor(AW::Color::random());

			const auto pts = AW::NumberHelper::randomInt(3, 8);
			const auto d = (AW::NumberHelper::PI * 2.0) / pts;
			for (auto i = 0; i < pts; ++i)
			{
				const auto x = std::cos(i * d) * dim;
				const auto y = std::sin(i * d) * dim;
				poly->addScreenPoint(x, y);
			}
			poly->setScreenRotation(AW::NumberHelper::random(360));

			poly->setScreenPosition((modules->screen->getWidth() / 2.0) + AW::NumberHelper::random(-20000.0, 20000.0), AW::NumberHelper::random(modules->screen->getHeight() / 2.0 - 10000.0));
			contentContainer->add(poly);
		}

		for (int i = 0; i < 5; ++i)
		{
			const auto c = std::make_shared<AW::Chain>();
			c->setDynamic(true);
			c->setScreenPosition(modules->screen->getWidth() / 2.0 + AW::NumberHelper::random(-400.0, 400.0), modules->screen->getHeight() / 2.0 + AW::NumberHelper::random(-800.0, -1000.0));
			c->setColor(blockColorGenerator.getBlockColor());
			for (int i = 0, numPoints = AW::NumberHelper::randomInt(4, 8); i < numPoints; ++i)
			{
				c->addScreenPoint(AW::NumberHelper::random(0, 300.0), AW::NumberHelper::random(0, 300.0));
			}

			contentContainer->add(c);
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
			if (contentContainer->getChildren().size() < 800)
			{
				const auto b = std::make_shared<AW::Box>();
				b->setAlpha(0.0);

				b->setDynamic(true);
				b->setScreenSize(50, 50);
				b->setScreenPosition(modules->screen->getWidth() / 2.0 + AW::NumberHelper::random(-400.0, 400.0), modules->screen->getHeight() / 2.0 + AW::NumberHelper::random(-800.0, -1000.0));
				b->setColor(blockColorGenerator.getBlockColor());
				b->setTexture("prop-blocks");

				const auto fragmentShader = modules->shader->getShader({ "block" }, true);

				b->setFragmentShader(fragmentShader);
				b->getFragmentShader()->setFloatIUParam("clipX", 32.0);
				b->getFragmentShader()->setFloatIUParam("clipY", 0.0);
				b->getFragmentShader()->setFloatIUParam("clipWidth", 64.0);
				b->getFragmentShader()->setFloatIUParam("clipHeight", 64.0);
				b->getFragmentShader()->setFloatIUParam("blockBorderSize", 2.0);
				b->getFragmentShader()->setFloatIUParam("blockEffect", 0.5);
				b->getFragmentShader()->setFloatIUParam("blockEffectP", 0.25);
				b->getFragmentShader()->setFloatIUParam("blockEffectG", 0.4);
				b->getFragmentShader()->setFloatIUParam("blockCenterFill", AW::NumberHelper::random(0.5, 1.0));
				b->getFragmentShader()->setFloatIUParam("fScanlineRetroAmount", 0.25);

				contentContainer->add(b);
			}

			modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		}
	}

	void TestScene2::onEnterFrame(const double& deltaTime)
	{
		if (contentContainer == nullptr) rebuild();

		for (const auto c : contentContainer->getChildrenOfType<AW::Box>())
		{
			if (c->getWorldY() < -20.0)
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

		if (key == AWKey::SIX)
		{
			modules->physic->setWorldGravity(0);
		}

		if (key == AWKey::SEVEN)
		{
			const auto dim = 50.0;
			for (auto i = 0; i < 30; ++i)
			{
				const auto poly = std::make_shared<AW::Poly>();
				poly->setColor(AW::Color::random());

				const auto pts = AW::NumberHelper::randomInt(3, 8);
				const auto d = (AW::NumberHelper::PI * 2.0) / pts;
				for (auto i = 0; i < pts; ++i)
				{
					const auto x = std::cos(i * d) * dim;
					const auto y = std::sin(i * d) * dim;
					poly->addScreenPoint(x, y);
				}
				poly->centerBalancePoints();
				poly->setScreenRotation(AW::NumberHelper::random(360));

				poly->setScreenPosition((modules->screen->getWidth() / 2.0) + AW::NumberHelper::random(-2000.0, 2000.0), AW::NumberHelper::random(modules->screen->getHeight() / 2.0 - 10000.0));
				contentContainer->add(poly);
			}

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
