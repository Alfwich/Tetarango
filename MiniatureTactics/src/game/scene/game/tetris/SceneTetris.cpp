#include "SceneTetris.h"
#include "scene/game/SceneMainGame.h"
#include "generator/block/GeneratorBlock.h"

namespace
{
	MTGame::GeneratorBlock blockColorGenerator;
}

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

		modules->input->keyboard->registerKeys({ SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, SDL_SCANCODE_UP, SDL_SCANCODE_BACKSPACE }, weak_from_this());
		keyRepeatTimer = modules->time->createTimer(MT::TimeScope::Game);
		pieceTimer = modules->time->createTimer(MT::TimeScope::Game);
		keyRepeatTimer->start();

		enableEnterFrame();
	}

	void SceneTetris::onCreateChildren()
	{
		board = std::make_shared<Board>(20, 20);
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
		if ((isLeftDown || isRightDown) && keyRepeatTimer->isAboveThresholdAndRestart(60))
		{
			if (isLeftDown)
			{
				board->moveCurrentPiece(-1);
			}
			else if (isRightDown)
			{
				board->moveCurrentPiece(1);
			}
		}

		if (!board->getHadActivePiece() && !board->getHasFailedToPlacePiece())
		{
			pieceTimer->startIfNotRunning();
		}

		if (pieceTimer->isAboveThresholdAndStop(500))
		{
			board->addTetromino(blockColorGenerator.getTetromino());
		}
	}

	void SceneTetris::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_1:
			//board->addTetromino(blockColorGenerator.getTetromino());
			break;

		case SDL_SCANCODE_2:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 0.25);
			break;

		case SDL_SCANCODE_3:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 5.0);
			break;

		case SDL_SCANCODE_4:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 1.0);
			break;

		case SDL_SCANCODE_5:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 1000.0);
			break;

		case SDL_SCANCODE_LEFT:
			isLeftDown = true;
			break;

		case SDL_SCANCODE_RIGHT:
			isRightDown = true;
			break;

		case SDL_SCANCODE_DOWN:
			board->enableFastFall();
			break;

		case SDL_SCANCODE_UP:
			board->rotatePiece();
			break;

		case SDL_SCANCODE_BACKSPACE:
			board->resetBoard();
			break;
		}
	}

	void SceneTetris::onKeyReleased(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_LEFT:
			isLeftDown = false;
			break;

		case SDL_SCANCODE_RIGHT:
			isRightDown = false;
			break;

		case SDL_SCANCODE_DOWN:
			board->disableFastFall();
			break;
		}
	}

}