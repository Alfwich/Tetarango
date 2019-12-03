#pragma once

#include "scene/BaseScene.h"
#include "actor/board/Board.h"
#include "gui/camera/GameCamera.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/CachedImage.h"
#include "ui/renderable/element/Rectangle.h"

namespace MTGame
{

	class SceneTetris : public BaseScene
	{
		int score = 0, deferPostRenderCount = 0;
		bool isLeftDown = false, isRightDown = false, saveScreenOnNextEnterFrame = false;
		std::shared_ptr<MT::Timer> keyRepeatTimer;
		std::shared_ptr<MT::Text> scoreText;
		std::shared_ptr<MT::ParticleSystem> particleSystem;
		std::shared_ptr<MT::ParticleSystem> blockParticleSystem;
		std::shared_ptr<Board> board;
		std::shared_ptr<Board> previewBoard;
		std::shared_ptr<GameCamera> camera;

		void updateScoreText();

	public:
		SceneTetris();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		virtual std::shared_ptr<MT::SerializationClient> doSerialize(MT::SerializationHint hint);

		void onEnterFrame(double deltaTime);
		void onPostRender();
		void onKeyPressed(SDL_Scancode key);
		void onKeyReleased(SDL_Scancode key);
		void onAboutToSave();
	};
}
