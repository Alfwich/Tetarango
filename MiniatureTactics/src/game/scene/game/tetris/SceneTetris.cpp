#include "SceneTetris.h"
#include "scene/game/SceneMainGame.h"

namespace MTGame
{
	SceneTetris::SceneTetris() : BaseScene(SceneGame::Tetris)
	{
		enableSerialization<SceneTetris>();
	}

	void SceneTetris::onInitialAttach()
	{
		const auto gameScene = findFirstInParentChain<SceneMainGame>();
		if (gameScene != nullptr)
		{
			gameScene->enableMenu();
		}

		modules->input->keyboard->registerKey(SDL_SCANCODE_1, weak_from_this());
		spawnTimer = modules->time->createTimer(MT::TimeScope::Game);
		spawnTimer->start();

		enableEnterFrame();
	}

	void SceneTetris::onCreateChildren()
	{
		board = std::make_shared<Board>(30, 30);
		board->name = "board";
		add(board);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(4.0, 1.0);
		camera->setDefaultsAndReset(2.0, 0.0, 0.0);
		camera->setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(MT::TimeScope::Camera);
		add(camera);
	}

	void SceneTetris::onChildrenHydrated()
	{
		board = findChildWithName<Board>("board");
		camera = findChildWithName<GameCamera>("camera");
	}

	void SceneTetris::onEnterFrame(double deltaTime)
	{
		if (spawnTimer->isAboveThresholdAndRestart(250))
		{
			onKeyPressed(SDL_SCANCODE_1);
		}
	}

	void SceneTetris::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_1:
			const auto block = std::make_shared<Block>();
			block->setColor(MT::Color::random());
			block->blockX = MT::NumberHelper::randomInt(0, 30);
			block->blockY = 0;

			if (board != nullptr)
			{
				board->add(block);
			}
			break;
		}
	}

}