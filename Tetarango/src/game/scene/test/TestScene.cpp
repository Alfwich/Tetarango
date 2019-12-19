#include "TestScene.h"

#include <random>
#include "GameImports.h"

namespace
{
	const char* backButtonId = "back_button";
	const std::string player = "player";
}

namespace AWGame
{

	TestScene::TestScene() : AW::Scene("test_scene")
	{
		rebuildOnLoad = true;
		registerSerialization<TestScene>();
	}

	void TestScene::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(AW::TimeScope::Game);

		modules->input->keyboard->registerKey(SDL_SCANCODE_ESCAPE, weak_from_this());
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());
	}

	void TestScene::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
	}

	void TestScene::onCreateChildren()
	{
		const auto mainGameMenu = std::make_shared<GameMainMenu>();
		mainGameMenu->zIndex = 20;
		mainGameMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		mainGameMenu->renderPositionMode = AW::RenderPositionMode::Absolute;
		mainGameMenu->visible = false;
		add(mainGameMenu);

		const auto backdrop = std::make_shared<AW::Backdrop>();
		backdrop->setTexture("test-scene-background");
		backdrop->setSizeToEffectiveInfinity();
		add(backdrop);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(4.0, 1.0);
		camera->setDefaultsAndReset(2.0, 0.0, 0.0);
		camera->setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		add(camera);

		auto parentEle = std::make_shared<AW::Rectangle>();
		auto hatEle = std::make_shared<AW::Rectangle>();
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

			auto pRect = std::make_shared<AW::Rectangle>();
			pRect->name = "left";
			pRect->setColor(0, 0, 0xff);
			pRect->setSizeAndPosition(0.0, hatEle->getHeight() / 2.0, 50.0, 50.0);
			hatEle->add(pRect);

			{
				std::shared_ptr<AW::Rectangle> rect = std::make_shared<AW::Rectangle>();
				rect->name = "middle";
				rect->setColor(0xff, 0xff, 0xff);
				rect->setSizeAndPosition(hatEle->getWidth() / 2.0, hatEle->getHeight() / 2.0, 50.0, 50.0);
				hatEle->add(rect);
			}

			{
				std::shared_ptr<AW::Rectangle> rect = std::make_shared<AW::Rectangle>();
				rect->name = "right";
				rect->setColor(0, 0xff, 0xff);
				rect->setSizeAndPosition(hatEle->getWidth(), hatEle->getHeight() / 2.0, 50.0, 50.0);
				hatEle->add(rect);
			}

			{
				std::shared_ptr<AW::Rectangle> cRect = std::make_shared<AW::Rectangle>();
				cRect->setColor(0, 0, 0);
				cRect->setSizeAndPosition(pRect->getWidth() / 2.0, pRect->getHeight() / 2.0, 3.0, 3.0);
				pRect->add(cRect);
			}

			{
				auto text = std::make_shared<AW::Text>("medium", 28);
				text->setText("Tetris Test");
				text->setTextColor(0xff, 0xff, 0xff);
				text->setPosition(hatEle->getWidth() / 2.0, hatEle->getHeight() / 2.0);
				hatEle->add(text);
			}

			{
				auto board = std::make_shared<Board>();
				board->name = "board";
				add(board);

				for (auto i = 0; i < 5; ++i)
				{
					auto newBlock = std::make_shared<AWGame::Block>();
					newBlock->setColor(AW::Color::random());
					newBlock->setPosition(i * 64, 0);
					newBlock->zIndex = 1;
					board->add(newBlock);
				}
			}

		}
	}

	void TestScene::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene::onEnterFrame(const double& deltaTime) { }

	void TestScene::onKeyPressed(SDL_Scancode key)
	{
		if (key == SDL_SCANCODE_ESCAPE)
		{
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
		}
	}

	void TestScene::onMouseButtonLeftDown()
	{
	}
}
