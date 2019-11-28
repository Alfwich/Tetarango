#pragma once

#include "scene/BaseScene.h"
#include "actor/board/Board.h"
#include "gui/camera/GameCamera.h"

namespace MTGame
{

	class SceneTetris : public BaseScene
	{

		bool isLeftDown = false, isRightDown = false;
		std::shared_ptr<MT::Timer> keyRepeatTimer;
		std::shared_ptr<Board> board;
		std::shared_ptr<Board> previewBoard;
		std::shared_ptr<GameCamera> camera;

	public:
		SceneTetris();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(double deltaTime);
		void onKeyPressed(SDL_Scancode key);
		void onKeyReleased(SDL_Scancode key);
	};
}
