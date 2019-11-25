#include "SceneTetris.h"
#include "actor/board/Board.h"
#include "scene/game/SceneMainGame.h"

namespace MTGame
{
	SceneTetris::SceneTetris() : BaseScene(SceneGame::Tetris)
	{
		enableSerialization<SceneTetris>();
	}

	void SceneTetris::onInitialAttach()
	{
		enableEnterFrame();
		const auto gameScene = findFirstInParentChain<SceneMainGame>();
		if (gameScene == nullptr)
		{
			return;
		}

		gameScene->enableMenu();
	}

	void SceneTetris::onCreateChildren()
	{
		auto board = std::make_shared<Board>();
		board->name = "board";
		add(board);
	}

	void SceneTetris::onChildrenHydrated()
	{
	}

	void SceneTetris::onEnterFrame(double deltaTime)
	{
		const auto board = findChildWithName<Board>("board");
		if (board != nullptr)
		{
			board->movePosition(30 * deltaTime / 1000, 0.0);
		}
	}

	void SceneTetris::onKeyPressed(SDL_Scancode key)
	{
	}

}