#pragma once

#include "scene/BaseScene.h"
#include "actor/board/Board.h"
#include "gui/camera/GameCamera.h"

namespace MTGame
{

	class SceneTetris : public BaseScene
	{

		std::shared_ptr<MT::Timer> spawnTimer;
		std::shared_ptr<Board> board;
		std::shared_ptr<GameCamera> camera;

	public:
		SceneTetris();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(double deltaTime);
		void onKeyPressed(SDL_Scancode key);
	};
}
