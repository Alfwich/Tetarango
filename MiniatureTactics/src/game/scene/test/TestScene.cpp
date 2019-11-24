#include "TestScene.h"

#include <random>
#include "GameImports.h"

namespace
{
	const char* backButtonId = "back_button";
	const std::string player = "player";
}

namespace MTGame
{

	TestScene::TestScene() : MT::Scene("test_scene")
	{
		rebuildOnLoad = true;
		enableSerialization<TestScene>();
	}

	void TestScene::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(MT::TimeScope::Game);

		modules->input->keyboard->registerKey(SDL_SCANCODE_ESCAPE, weak_from_this());
		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, weak_from_this());

		spawnTimer = modules->time->createTimer(MT::TimeScope::Game);
		spawnTimer->start();
	}

	void TestScene::onAttach()
	{
		modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 1.0);
	}

	void TestScene::onCreateChildren()
	{
		const auto mainGameMenu = std::make_shared<GameMainMenu>();
		mainGameMenu->zIndex = 20;
		mainGameMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		mainGameMenu->renderPositionMode = MT::RenderPositionMode::Absolute;
		mainGameMenu->visible = false;
		add(mainGameMenu);

		const auto backdrop = std::make_shared<MT::Backdrop>();
		backdrop->setTexture("test-scene-background");
		backdrop->setSizeToEffectiveInfinity();
		add(backdrop);

		screenRectContainer = std::make_shared<MT::Container>();
		screenRectContainer->zIndex = 10;
		screenRectContainer->renderPositionMode = MT::RenderPositionMode::Absolute;
		add(screenRectContainer);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(4.0, 1.0);
		camera->setDefaultsAndReset(2.0, 0.0, 0.0);
		camera->setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(MT::TimeScope::Camera);
		add(camera);

		auto parentEle = std::make_shared<MT::Rectangle>();
		auto hatEle = std::make_shared<MT::Rectangle>();
		{
			parentEle->name = "rotation-root";
			parentEle->setPosition(500, 500);
			parentEle->setRotation(0.0);
			parentEle->setSize(100, 100);
			parentEle->zIndex = 8;
			add(parentEle);

			hatEle->setColor(0, 0xff, 0);
			hatEle->name = "hatEle";
			hatEle->setPosition(parentEle->getWidth() / 2, -400);
			hatEle->setSize(400, 50);
			parentEle->add(hatEle);

			auto pRect = std::make_shared<MT::Rectangle>();
			pRect->name = "left";
			pRect->setColor(0, 0, 0xff);
			pRect->setSizeAndPosition(0.0, hatEle->getHeight() / 2.0, 50.0, 50.0);
			hatEle->add(pRect);

			{
				std::shared_ptr<MT::Rectangle> rect = std::make_shared<MT::Rectangle>();
				rect->name = "middle";
				rect->setColor(0xff, 0xff, 0xff);
				rect->setSizeAndPosition(hatEle->getWidth() / 2.0, hatEle->getHeight() / 2.0, 50.0, 50.0);
				hatEle->add(rect);
			}

			{
				std::shared_ptr<MT::Rectangle> rect = std::make_shared<MT::Rectangle>();
				rect->name = "right";
				rect->setColor(0, 0xff, 0xff);
				rect->setSizeAndPosition(hatEle->getWidth(), hatEle->getHeight() / 2.0, 50.0, 50.0);
				hatEle->add(rect);
			}

			{
				std::shared_ptr<MT::Rectangle> cRect = std::make_shared<MT::Rectangle>();
				cRect->setColor(0, 0, 0);
				cRect->setSizeAndPosition(pRect->getWidth() / 2.0, pRect->getHeight() / 2.0, 3.0, 3.0);
				pRect->add(cRect);
			}

			{
				auto text = std::make_shared<MT::Text>("medium", 28);
				text->setText("Tetris Test");
				text->setTextColor(0xff, 0xff, 0xff);
				text->setPosition(hatEle->getWidth() / 2.0, hatEle->getHeight() / 2.0);
				hatEle->add(text);
			}

			{

				std::shared_ptr<MTGame::Block> block;
				for (auto i = 0; i < 5; ++i)
				{
					auto newBlock = std::make_shared<MTGame::Block>();
					if (block == nullptr) {
						newBlock->name = "block-root";
						newBlock->setPosition(0, 0);
					}
					else
					{
						newBlock->toRightOf(block, block->getHalfWidth());
					}
					newBlock->setColor(MT::Color::random());
					add(newBlock);
					block = newBlock;
				}
			}
		}

		/*
		for (auto teamId = 0; teamId < 10; ++teamId)
		{
			const auto spawnX = MT::NumberHelper::random(-2000, 2000),
				spawnY = MT::NumberHelper::random(-2000, 2000);
			for (auto i = 0; i < 5; ++i)
			{
				auto npc = std::make_shared<PawnHuman>();
				npc->setPosition(spawnX + MT::NumberHelper::random(-100, 100), spawnY + MT::NumberHelper::random(-100, 100));
				npc->setTeam(teamId);
				npc->setController(std::make_shared<PawnControllerAICombatSimple>());
				generator.provisionCombatPawn(npc);
				add(npc);
			}
		}

		for (auto i = 0; i < 20; ++i)
		{
			auto npc = std::make_shared<PawnHuman>();
			npc->setPosition(MT::NumberHelper::random(-2000, 2000), MT::NumberHelper::random(-2000, 2000));
			npc->setController(std::make_shared<PawnControllerAINpc>());
			generator.provisionNonCombatPawn(npc);
			add(npc);
		}

		auto globalSystem = std::make_shared<MT::ParticleSystem>();
		globalSystem->setGlobalReceiver(true);
		add(globalSystem);
	}

	void TestScene::onEnterFrame(double frameTime)
	{
		auto child = findChildWithName<MT::ApplicationObject>("rotation-root");
		auto renderable = std::dynamic_pointer_cast<MT::Renderable>(child);
		if (renderable == nullptr)
		{
			return;
		}

		auto rotation = 30 * (frameTime / 1000.0);
		renderable->rotate(rotation * 1);

		if (player != nullptr && camera != nullptr)
		{
			camera->setX(player->getX());
			camera->setY(player->getY());
		}

		if (false && spawnTimer->getTicks() > 1000)
		{
			for (auto i = 0; i < 10; ++i)
			{
				auto npc = std::make_shared<PawnHuman>();
				npc->setPosition(MT::NumberHelper::random(-1000, 1000), MT::NumberHelper::random(-1000, 1000));
				npc->setTeam(1);
				npc->setController(std::make_shared<PawnControllerAICombatSimple>());
				generator.provisionCombatPawn(npc);
				add(npc);
			}
			spawnTimer->start();
		}

		screenRectContainer->destroyChildren();
		for (const auto child : children)
		{
			const auto pawnPtr = std::dynamic_pointer_cast<PawnHuman>(child);
			if (pawnPtr != nullptr)
			{
				const auto sR = child->getScreenRect();
				const auto r = std::make_shared<MT::Rectangle>();
				r->setSizeAndPosition(sR->x, sR->y, sR->w, sR->h);
				r->movePosition(sR->w / 2.0, sR->h / 2.0);
				screenRectContainer->add(r);
			}
		}
		*/
	}

	void TestScene::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene::onEnterFrame(double deltaTime)
	{
		auto child = findChildWithName<MT::ApplicationObject>("block-root");
		auto renderable = std::dynamic_pointer_cast<MT::Renderable>(child);
		if (renderable != nullptr)
		{
			renderable->rotate(deltaTime);
		}
	}

	void TestScene::onKeyPressed(SDL_Scancode key)
	{
		if (key == SDL_SCANCODE_ESCAPE)
		{
			const auto applicationSceneContainer = findFirstInParentChain<MT::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
		}
	}

	void TestScene::onMouseButtonLeftDown()
	{
	}
}
