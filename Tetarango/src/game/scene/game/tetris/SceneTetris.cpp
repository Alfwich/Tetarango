#include "SceneTetris.h"
#include "scene/game/SceneMainGame.h"
#include "generator/block/GeneratorBlock.h"
#include "prop/particles/space/ParticleSpaceBackgroundFactory.h"
#include "prop/particles/block/BlockParticleFactory.h"
#include "ui/renderable/element/DisplayBuffer.h"

namespace
{
	AWGame::GeneratorBlock blockColorGenerator;
	const auto musicName = "tetris-music";
	const auto clearRowSoundName = "tetris-clear-row";
	const auto gameOverSoundName = "tetris-game-over";
	const auto tetarangoSoundName = "tetris-tetarango";
}

namespace AWGame
{
	SceneTetris::SceneTetris() : BaseScene(SceneGame::Tetris)
	{
		registerGameObject<SceneTetris>();
	}

	void SceneTetris::onLoadResources()
	{
		modules->sound->loadMusic("res/sound/tetris.mp3", musicName);

		modules->sound->loadSoundClip("res/sound/clear-row.wav", clearRowSoundName);
		modules->sound->loadSoundClip("res/sound/game-over.wav", gameOverSoundName);
		modules->sound->loadSoundClip("res/sound/tetarango.wav", tetarangoSoundName);
	}

	void SceneTetris::updateScoreText()
	{
		if (scoreText == nullptr)
		{
			return;
		}

		scoreText->setText("Score: " + AW::StringHelper::padLeft(std::to_string(score), 10, '0'));
	}

	void SceneTetris::onInitialAttach()
	{
		const auto gameScene = findFirstInParentChain<SceneMainGame>();
		if (gameScene != nullptr)
		{
			gameScene->enableMenu();
		}

		modules->input->keyboard->registerKeys({ SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, SDL_SCANCODE_UP, SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_0 }, weak_from_this());

		keyRepeatTimer = modules->time->createTimer(AW::TimeScope::Game);
		keyRepeatTimer->start();


		enableEnterFrame();
	}

	void SceneTetris::onAttach()
	{
		if (!board->getHasFailedToPlacePiece())
		{
			modules->sound->playMusic(musicName);
		}
		else
		{
			modules->sound->stopMusic(musicName);
		}
		particleSystem->start();
	}

	void SceneTetris::onDetach()
	{
		modules->sound->stopMusic(musicName);
	}

	void SceneTetris::onCreateChildren()
	{
		const auto cached = std::make_shared<AW::DisplayBuffer>();
		cached->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		cached->renderUpdateMode = AW::RenderUpdateMode::EveryFrame;
		cached->renderMode = AW::RenderMode::ChildrenOnly;
		cached->topLeftAlignSelf();
		add(cached);

		const auto testBlock = std::make_shared<Block>();
		board = std::make_shared<Board>(10, 24);
		board->setCellSize(testBlock->getWidth(), testBlock->getHeight());
		board->centerAlignWithin(cached);
		board->name = "board";
		cached->add(board);

		previewBoard = std::make_shared<Board>(5, 5);
		previewBoard->setCellSize(testBlock->getWidth(), testBlock->getHeight());
		previewBoard->name = "preview-board";
		previewBoard->disableBoardFalling();
		previewBoard->addTetromino(blockColorGenerator.getTetromino());
		cached->add(previewBoard);

		previewBoard->toRightTopOf(board, 10);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->listener = weak_from_this();
		add(camera);

		scoreText = std::make_shared<AW::Text>();
		scoreText->name = "score-text";
		scoreText->setTextColor(255, 255, 255);
		scoreText->setFont("medium", 48);
		updateScoreText();
		scoreText->toRightOf(board, 10);
		cached->add(scoreText);

		particleSystem = std::make_shared<AW::ParticleSystem>();
		particleSystem->zIndex = -5;
		particleSystem->name = "p-system";
		particleSystem->setSize(modules->screen->getWidth() * 2.0, modules->screen->getHeight() * 2.0);
		particleSystem->setParticleFactory(std::make_shared<ParticleSpaceBackgroundParticleFactory>());
		particleSystem->setParticlesPerSecond(2);
		particleSystem->emitImmediately(40);
		cached->add(particleSystem);

		blockParticleSystem = std::make_shared<AW::ParticleSystem>();
		blockParticleSystem->zIndex = -5;
		blockParticleSystem->name = "p-b-system";
		blockParticleSystem->matchSizeAndCenter(board, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		cached->add(blockParticleSystem);
	}

	void SceneTetris::onChildrenHydrated()
	{
		board = findChildWithName<Board>("board");
		previewBoard = findChildWithName<Board>("preview-board");
		camera = findChildWithName<GameCamera>("camera");
		camera->listener = weak_from_this();
		scoreText = findChildWithName<AW::Text>("score-text");
		particleSystem = findChildWithName<AW::ParticleSystem>("p-system");
		particleSystem->emitImmediately(40);
		blockParticleSystem = findChildWithName<AW::ParticleSystem>("p-b-system");
	}

	std::shared_ptr<AW::SerializationClient> SceneTetris::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__scene__", hint);
		score = client->serializeInt("score", score);

		return BaseScene::doSerialize(hint);
	}

	void SceneTetris::onEnterFrame(const double& deltaTime)
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
			const auto newPiece = blockColorGenerator.getTetromino();
			previewBoard->addTetromino(newPiece);
		}

		if (board->getHasFailedToPlacePiece() && !hasPlayedGameOverSound)
		{
			modules->sound->stopMusic(musicName, 50);
			modules->sound->playSoundClip(gameOverSoundName);
			hasPlayedGameOverSound = true;
		}

		auto eliminatedPieces = board->getEliminatedPieces();
		if (eliminatedPieces.size() > 0)
		{
			const auto blockParticleFactory = std::make_shared<BlockParticleFactory>();
			auto blockScore = 5;
			if (eliminatedPieces.size() > 35)
			{
				modules->sound->playSoundClip(tetarangoSoundName);
			}
			else
			{
				modules->sound->playSoundClip(clearRowSoundName);
			}

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

	void SceneTetris::onTimeoutCalled(int id)
	{
		if (id == saveScreenshotTimeoutId)
		{
			auto imageCatcher = std::make_shared<AW::ScreenImage>();
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

			saveScreenshotTimeoutId = 0;
		}
	}

	void SceneTetris::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_0:
		{
			for (const auto cached : getChildrenOfType<AW::DisplayBuffer>())
			{
				if (cached->renderMode == AW::RenderMode::CachedElement)
				{
					cached->renderMode = AW::RenderMode::ChildrenOnly;
				}
				else
				{
					cached->renderMode = AW::RenderMode::CachedElement;
				}
			}
		}
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
			modules->sound->playSoundClip(clearRowSoundName);
			hasPlayedGameOverSound = false;
			modules->sound->playMusic(musicName);
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
		setTimeout(100.0, &saveScreenshotTimeoutId);
	}

	void SceneTetris::onCameraUpdate()
	{
		setScale(camera->getZoom());
		setPosition(-camera->getX(), -camera->getY());
	}
}