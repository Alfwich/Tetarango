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
		keyRepeatTimer->start();

		enableEnterFrame();
	}

	void SceneTetris::onCreateChildren()
	{
		board = std::make_shared<Board>(10, 24);
		board->name = "board";
		add(board);

		previewBoard = std::make_shared<Board>(5, 5);
		previewBoard->name = "preview-board";
		previewBoard->disableBoardFalling();
		previewBoard->addTetromino(blockColorGenerator.getTetromino());
		add(previewBoard);

		previewBoard->toRightTopOf(board, 10);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		add(camera);
	}

	void SceneTetris::onChildrenHydrated()
	{
		board = findChildWithName<Board>("board");
		previewBoard = findChildWithName<Board>("preview-board");
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

		if (!board->getHasFailedToPlacePiece() && !board->hasAnActivePiece() && previewBoard->hasAnActivePiece())
		{
			board->addTetromino(previewBoard->getCurrentBlock());
			previewBoard->addTetromino(blockColorGenerator.getTetromino());
		}
	}

	void SceneTetris::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_1:
			board->enableFastFall();
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