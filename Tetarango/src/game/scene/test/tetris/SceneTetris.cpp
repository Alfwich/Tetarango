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
		setShouldRebuildOnLoad();
		setTimeScope(AW::TimeScope::Game);
		registerGameObject<SceneTetris>(__FUNCTION__);
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
		modules->input->keyboard->registerKeys({ AWKey::ESCAPE, AWKey::ONE, AWKey::TWO, AWKey::THREE, AWKey::FOUR, AWKey::FIVE, AWKey::LEFT, AWKey::RIGHT, AWKey::DOWN, AWKey::UP, AWKey::BACKSPACE, AWKey::ZERO }, weak_from_this());

		keyRepeatTimer = modules->time->createTimer();
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
		const auto cached = std::make_shared<AW::Container>();
		cached->setSize(modules->screen->getWidth(), modules->screen->getHeight());
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
		camera->setDefaultZoomAndAnchorPoint(1.0, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->listener = weak_from_this();
		cached->add(camera);

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
		blockParticleSystem->setSizeAndCenter(board, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
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

	void SceneTetris::captureScreen()
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

	void SceneTetris::onKeyPressed(AWKey key)
	{
		if (key == AWKey::ESCAPE)
		{
			destroyChildren();
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::SavedGamesMenu));
		}

		switch (key)
		{
		case AWKey::ZERO:
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

		case AWKey::LEFT:
			isLeftDown = true;
			break;

		case AWKey::RIGHT:
			isRightDown = true;
			break;

		case AWKey::DOWN:
			board->enableFastFall();
			break;

		case AWKey::UP:
			board->rotatePiece();
			break;

		case AWKey::BACKSPACE:
			score = 0;
			updateScoreText();
			board->resetBoard();
			modules->sound->playSoundClip(clearRowSoundName);
			hasPlayedGameOverSound = false;
			modules->sound->playMusic(musicName);
			break;
		}
	}

	void SceneTetris::onKeyReleased(AWKey key)
	{
		switch (key)
		{
		case AWKey::LEFT:
			isLeftDown = false;
			break;

		case AWKey::RIGHT:
			isRightDown = false;
			break;

		case AWKey::DOWN:
			board->disableFastFall();
			break;
		}
	}

	void SceneTetris::onCameraUpdate()
	{
	}
}