#pragma once

#include "scene/BaseScene.h"
#include "actor/board/Board.h"
#include "gui/camera/GameCamera.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/ScreenImage.h"
#include "ui/renderable/primitive/Rectangle.h"

namespace AWGame
{

	class SceneTetris : public BaseScene, public AW::ICameraListener
	{
		int score = 0, saveScreenshotTimeoutId = 0;
		bool isLeftDown = false, isRightDown = false, saveScreenOnNextEnterFrame = false, hasPlayedGameOverSound = false;
		std::shared_ptr<AW::Timer> keyRepeatTimer;
		std::shared_ptr<AW::Text> scoreText;
		std::shared_ptr<AW::ParticleSystem> particleSystem;
		std::shared_ptr<AW::ParticleSystem> blockParticleSystem;
		std::shared_ptr<Board> board;
		std::shared_ptr<Board> previewBoard;
		std::shared_ptr<GameCamera> camera;

		void updateScoreText();
		void captureScreen();

	public:
		SceneTetris();

		void onLoadResources();

		void onInitialAttach();
		void onAttach();
		void onDetach();
		void onCreateChildren();
		void onChildrenHydrated();

		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);

		void onEnterFrame(const double& deltaTime);
		void onKeyPressed(SDL_Scancode key);
		void onKeyReleased(SDL_Scancode key);
		void onAboutToSave();
		void onCameraUpdate();
	};
}
