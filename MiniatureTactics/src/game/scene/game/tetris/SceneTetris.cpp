#include "SceneTetris.h"
#include "scene/game/SceneMainGame.h"
#include "generator/block/GeneratorBlock.h"
#include "prop/particles/space/ParticleSpaceBackgroundFactory.h"
#include "prop/particles/block/BlockParticleFactory.h"

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

	void SceneTetris::updateScoreText()
	{
		if (scoreText == nullptr)
		{
			return;
		}

		scoreText->setText("Score: " + MT::StringHelper::padLeft(std::to_string(score), 10, '0'));
	}

	void SceneTetris::onInitialAttach()
	{
		const auto gameScene = findFirstInParentChain<SceneMainGame>();
		if (gameScene != nullptr)
		{
			gameScene->enableMenu();
		}

		modules->input->keyboard->registerKeys({ SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, SDL_SCANCODE_UP, SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_0 }, weak_from_this());

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

		scoreText = std::make_shared<MT::Text>();
		scoreText->name = "score-text";
		scoreText->setTextColor(255, 255, 255);
		scoreText->setFont("medium", 100);
		updateScoreText();
		scoreText->toRightOf(board, 10);
		add(scoreText);

		particleSystem = std::make_shared<MT::ParticleSystem>();
		particleSystem->zIndex = -5;
		particleSystem->name = "p-system";
		particleSystem->setSize(modules->screen->getWidth() * 2.0, modules->screen->getHeight() * 2.0);
		particleSystem->setParticleFactory(std::make_shared<ParticleSpaceBackgroundParticleFactory>());
		particleSystem->setParticlesPerSecond(2);
		particleSystem->emitImmediately(40);
		particleSystem->start();
		add(particleSystem);

		blockParticleSystem = std::make_shared<MT::ParticleSystem>();
		blockParticleSystem->zIndex = -5;
		blockParticleSystem->name = "p-b-system";
		blockParticleSystem->matchSizeAndCenter(board);
		add(blockParticleSystem);
	}

	void SceneTetris::onChildrenHydrated()
	{
		board = findChildWithName<Board>("board");
		previewBoard = findChildWithName<Board>("preview-board");
		camera = findChildWithName<GameCamera>("camera");
		scoreText = findChildWithName<MT::Text>("score-text");
		particleSystem = findChildWithName<MT::ParticleSystem>("p-system");
		particleSystem->emitImmediately(40);
		blockParticleSystem = findChildWithName<MT::ParticleSystem>("p-b-system");
	}

	std::shared_ptr<MT::SerializationClient> SceneTetris::doSerialize(MT::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__scene__", hint);
		score = client->serializeInt("score", score);

		return BaseScene::doSerialize(hint);
	}

	void SceneTetris::onEnterFrame(double deltaTime)
	{
		if ((isLeftDown || isRightDown) && keyRepeatTimer->isAboveThresholdAndRestart(120))
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

		auto eliminatedPieces = board->getEliminatedPieces();
		if (eliminatedPieces.size() > 0)
		{
			const auto blockParticleFactory = std::make_shared<BlockParticleFactory>();
			auto blockScore = 5;
			while (eliminatedPieces.size() > 0)
			{
				const auto block = eliminatedPieces.back();
				score += blockScore;
				blockScore++;
				blockParticleFactory->setModColor(block->getColor());
				blockParticleSystem->emitImmediatelyWithFactory(blockScore, blockParticleFactory);
				eliminatedPieces.pop_back();
			}
			updateScoreText();
		}
	}

	void SceneTetris::onPostRender()
	{
		if (deferPostRenderCount > 0)
		{
			deferPostRenderCount--;
			modules->event->registerPostRenderCallback(shared_from_this());
			return;
		}

		auto imageCatcher = std::make_shared<MT::CachedImage>();
		imageCatcher->setShouldSerializeImage(true);
		imageCatcher->captureWholeScreen();

		const auto currentSaveSlotId = std::stoi(modules->storage->getClient()->readSring(storagePath(StorePaths::System_CurrentSaveSlot)));
		std::string path;
		switch (currentSaveSlotId)
		{
		case 1:
			path = storagePath(StorePaths::System_SaveSlot1_Image);
			break;
		case 2:
			path = storagePath(StorePaths::System_SaveSlot2_Image);
			break;
		case 3:
			path = storagePath(StorePaths::System_SaveSlot3_Image);
			break;
		}

		if (!path.empty())
		{
			modules->storage->getClient()->writeString(path, modules->serialization->serialize(imageCatcher));
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
			score = 0;
			updateScoreText();
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

	void SceneTetris::onAboutToSave()
	{
		deferPostRenderCount = 1;
		modules->event->registerPostRenderCallback(shared_from_this());
	}
}