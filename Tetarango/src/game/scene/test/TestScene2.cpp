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
		modules->physic->setWorldFps(0, 120);
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
			platform = std::make_shared<AW::Body>();
			platform->setColor(64, 64, 64);
			platform->setSize(600.0, 100.0);
			platform->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
			contentContainer->add(platform);
		}

		const auto xOff = 700.0;
		const auto yOff = 0.0;
		{
			const auto b = std::make_shared<AW::Body>();
			b->setColor(64, 64, 64);
			b->setSize(500.0, 100.0);
			b->setRotation(45.0);
			b->setPosition(modules->screen->getWidth() / 2.0 - xOff, modules->screen->getHeight() / 2.0 - yOff);
			contentContainer->add(b);
		}

		{
			const auto b = std::make_shared<AW::Body>();
			b->setColor(64, 64, 64);
			b->setSize(500.0, 100.0);
			b->setRotation(-45.0);
			b->setPosition(modules->screen->getWidth() / 2.0 + xOff, modules->screen->getHeight() / 2.0 - yOff);
			contentContainer->add(b);
		}

		{
			follower = std::make_shared<AW::Body>();
			follower->setDynamicBody();
			follower->setColor(255, 255, 255);
			follower->setSize(200, 200);
			follower->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0 - 230.0);
			follower->setFriction(0.1);
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
				const auto b = std::make_shared<AW::Body>();
				b->setAlpha(0.0);

				if (AW::NumberHelper::chance(50))
				{
					const auto shader = modules->shader->getShader({ "block" }, true);
					shader->setFloatIUParam("clipX", 32.0);
					shader->setFloatIUParam("clipY", 0.0);
					shader->setFloatIUParam("clipWidth", 64.0);
					shader->setFloatIUParam("clipHeight", 64.0);
					shader->setFloatIUParam("blockBorderSize", 2.0);
					shader->setFloatIUParam("blockEffect", 0.5);
					shader->setFloatIUParam("blockEffectP", 0.25);
					shader->setFloatIUParam("blockEffectG", 0.4);
					shader->setFloatIUParam("blockCenterFill", AW::NumberHelper::random(0.5, 1.0));
					shader->setFloatIUParam("fScanlineRetroAmount", 0.25);
					b->setTexture("prop-blocks");
					b->setFragmentShader(shader);
				}
				else
				{
					b->setBodyType(AW::BodyType::Circle);
					const auto shader = modules->shader->getShader({ "f-color", "f-circle" }, true);
					shader->setFloatIUParam("fCircleEdge", 0.1);
					b->setFragmentShader(shader);
				}

				b->setDynamicBody();
				b->setSize(50, 50);
				b->setPosition(modules->screen->getWidth() / 2.0 + AW::NumberHelper::random(-400.0, 400.0), modules->screen->getHeight() / 2.0 + AW::NumberHelper::random(-800.0, -1000.0));
				b->setColor(blockColorGenerator.getBlockColor());
				b->setFriction(0.1);
				b->setDensity(0.1);

				contentContainer->add(b);
			}

			modules->event->registerTimeoutCallback(shared_from_this(), spawnMs);
		}
	}

	void TestScene2::onEnterFrame(const double& deltaTime)
	{
		float impulse = 2000.0;
		if (followButtonPressed)
		{
			const auto x1 = follower->getWorldRect()->x, y1 = follower->getWorldRect()->y, x2 = (double)modules->input->mouse->X(), y2 = (double)modules->input->mouse->Y();
			const auto v = std::atan2(y1 - y2, x1 - x2);
			const auto vX = x2 - x1;
			const auto vY = y1 - y2;
			const auto mag = std::sqrt(vX * vX + vY * vY);
			follower->applyForce(vX / mag, vY / mag, impulse * (deltaTime / 1000.0));
		}

		if (upPressed) follower->applyForce(0.0, 1.0, impulse * (deltaTime / 1000.0));
		else if (downPressed) follower->applyForce(0.0, -1.0, impulse * (deltaTime / 1000.0));
		else if (leftPressed) follower->applyForce(-1.0, 0.0, impulse * (deltaTime / 1000.0));
		else if (rightPressed) follower->applyForce(1.0, 0.0, impulse * (deltaTime / 1000.0));

		for (const auto c : contentContainer->getChildrenOfType<AW::Body>())
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
			auto targetR = platform->getRect();
			targetR.y += 50.0;
			tran->setLooping(true);
			tran->startTransition(platform, 500, targetR);
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
