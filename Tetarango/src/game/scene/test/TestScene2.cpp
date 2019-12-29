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

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(16000000.0, 0);
		camera->setDefaultZoomAndAnchorPoint(1.0, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		contentContainer->add(camera);

		b2Vec2 g(0, -10);
		world = new b2World(g);

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0, -10.0);
		groundBody = world->CreateBody(&groundBodyDef);

		b2PolygonShape groundBox;
		groundBox.SetAsBox(100.0, 10.0);

		groundBody->CreateFixture(&groundBox, 0.0f);

		for (int x = 0; x < 500; ++x)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(AW::NumberHelper::random(-30.0, 30.0), AW::NumberHelper::random(10.0, 50.0));
			auto bdy = world->CreateBody(&bodyDef);

			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(1.0f, 1.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 20.0;
			fixtureDef.friction = 0.8f;
			bdy->CreateFixture(&fixtureDef);

			auto dynamicRect = std::make_shared<AW::Rectangle>();
			dynamicRect->setSize(20.0, 20.0);
			dynamicRect->setColor(AW::Color::random());
			contentContainer->add(dynamicRect);

			objs.push_back(std::make_pair(bdy, dynamicRect));
		}

		const auto groundRect = std::make_shared<AW::Rectangle>();
		groundRect->setSize(2000.0, 200.0);
		groundRect->setColor(64, 64, 64);
		contentContainer->add(groundRect);
		obj1 = groundRect;

		modules->event->registerTimeoutCallback(shared_from_this(), 16);
	}

	void TestScene2::onLayoutChildren()
	{
		const auto xOffset = modules->screen->getWidth() / 2.0;
		const auto yOffset = modules->screen->getWidth() / 2.0;
		const auto groundPos = groundBody->GetPosition();
		const auto dynamicRot = groundBody->GetAngle();
		obj1->setPosition(groundPos.x * 10.0 + xOffset, groundPos.y * -10.0 + yOffset);

		for (const auto bodyToRect : objs)
		{
			const auto dynamicPos = bodyToRect.first->GetPosition();
			const auto dynamicRot = bodyToRect.first->GetAngle();

			bodyToRect.second->setPosition(dynamicPos.x * 10.0 + xOffset, dynamicPos.y * -10.0 + yOffset);
			bodyToRect.second->setRotation(dynamicRot * -AW::NumberHelper::PI);

		}
	}

	void TestScene2::onTimeoutCalled()
	{
		world->Step(16 / 1000.0f, 8, 3);
		modules->event->registerTimeoutCallback(shared_from_this(), 16);
		layout();
	}

	void TestScene2::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
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
			for (const auto bodyToRect : objs)
			{
				bodyToRect.first->ApplyForce(
					b2Vec2(AW::NumberHelper::random(-1000.0, 1000.0), AW::NumberHelper::random(-1000.0, 1000.0)),
					b2Vec2(0.0, 0.0),
					true
				);
			}

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
