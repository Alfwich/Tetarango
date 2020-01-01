#include "SceneMainGame.h"

#include "GameImports.h"

namespace
{
	const auto masterSceneContainerId = "global_scene_container";
	const auto starSystemId = "global_system";
	const auto mainMenuId = "global_game_menu";
	const auto globalTransitionId = "global_game_transition";
	const auto hudId = "global_hud";
	const auto playerGameStateId = "playerGameState";
}

namespace AWGame
{
	std::string SceneMainGame::getSaveSlotFilePath()
	{
		const auto currentSaveSlotId = std::stoi(modules->storage->getClient()->readSring(storagePath(StorePaths::System_CurrentSaveSlot)));

		std::string storageFileLocation;
		switch (currentSaveSlotId)
		{
		case 1:
			return filePath(StorePaths::System_SaveSlot1);
			break;

		case 2:
			return filePath(StorePaths::System_SaveSlot2);
			break;

		case 3:
			return filePath(StorePaths::System_SaveSlot3);
			break;
		}

		return std::string();
	}

	SceneMainGame::SceneMainGame() : BaseScene(SceneGame::MainGame)
	{
		registerGameObject<SceneMainGame>(__FUNCTION__);
	}

	void SceneMainGame::onInitialAttach()
	{
		modules->input->keyboard->registerKey(SDL_SCANCODE_ESCAPE, baseSceneWeakThisRef());

		setTimeScope(AW::TimeScope::Game);
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 0.0);

		const auto currentSaveSlotId = std::stoi(modules->storage->getClient()->readSring(storagePath(StorePaths::System_CurrentSaveSlot)));

		switch (currentSaveSlotId)
		{
		case 1:
			storageScopeName = filePath(StorePaths::System_SaveSlot1);
			break;

		case 2:
			storageScopeName = filePath(StorePaths::System_SaveSlot2);
			break;

		case 3:
			storageScopeName = filePath(StorePaths::System_SaveSlot3);
			break;
		}

		bool hasLoaded = serializationClient->getBool(storagePath(StorePaths::HasLoaded));

		if (!hasLoaded)
		{
			const auto storageClient = modules->storage->getClient(storageScopeName);

			serializationClient->setInt(storagePath(StorePaths::SlotId), currentSaveSlotId);
			serializationClient->setBool(storagePath(StorePaths::HasLoaded), true);
			serializationClient->setInt(storagePath(StorePaths::GameState), 1);

			storageClient->writeInt(storagePath(StorePaths::SlotId), currentSaveSlotId);
		}
	}

	void SceneMainGame::onCreateChildren()
	{
		gameMainMenu = std::make_shared<GameMainMenu>();
		gameMainMenu->name = mainMenuId;
		gameMainMenu->zIndex = 18;
		gameMainMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		gameMainMenu->renderPositionMode = AW::RenderPositionMode::Absolute;
		gameMainMenu->visible = false;
		add(gameMainMenu);

		globalTransition = std::make_shared<TransitionFade>();
		globalTransition->name = globalTransitionId;
		globalTransition->setDuration(450.0);
		globalTransition->zIndex = 20;
		globalTransition->listener = std::dynamic_pointer_cast<INotifyOnFade>(shared_from_this());
		globalTransition->setSizeAndPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0, modules->screen->getWidth(), modules->screen->getHeight());
		globalTransition->renderPositionMode = AW::RenderPositionMode::Absolute;
		add(globalTransition);

		masterSceneContainer = std::make_shared<AW::SceneContainer>();
		masterSceneContainer->name = masterSceneContainerId;
		masterSceneContainer->notifyOnTransition = weak_from_this();
		add(masterSceneContainer);

		masterSceneContainer->add(std::make_shared<SceneTetris>());
		masterSceneContainer->add(std::make_shared<SceneOptionsMenu>());

		globalParticleSystem = std::make_shared<AW::ParticleSystem>();
		globalParticleSystem->name = starSystemId;
		globalParticleSystem->setGlobalReceiver(true);
		add(globalParticleSystem);

		masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Tetris));
		globalTransition->fadeOut();
	}

	void SceneMainGame::onDisplayProvisioned()
	{
		const auto currentScene = masterSceneContainer->getCurrentScene();
		if (currentScene != nullptr)
		{
			currentScene->rebuild();
		}

		gameMainMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		globalTransition->setSizeAndPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0, modules->screen->getWidth(), modules->screen->getHeight());
	}

	void SceneMainGame::onChildrenHydrated()
	{
		masterSceneContainer = findChildWithName<AW::SceneContainer>(masterSceneContainerId);
		globalParticleSystem = findChildWithName<AW::ParticleSystem>(starSystemId);
		gameMainMenu = findChildWithName<GameMainMenu>(mainMenuId);
		gameMainMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		gameMainMenu->hide();

		globalTransition = findChildWithName<TransitionFade>(globalTransitionId);
		globalTransition->listener = std::dynamic_pointer_cast<INotifyOnFade>(shared_from_this());
		globalTransition->fadeInImmediately();
		globalTransition->fadeOut();

		hud = findChildWithName<BaseHud>(hudId);
	}

	void SceneMainGame::onWorkError(AW::WORKER_ID workerId, AW::WorkerTaskCode code)
	{
		// TODO
	}

	void SceneMainGame::onWorkDone(AW::WORKER_ID workerId, AW::WorkerTaskCode code, std::shared_ptr<AW::AsyncResultBundle> result)
	{
		switch (code)
		{
		case AW::WorkerTaskCode::SERIALIZATION:
		{
			const auto scope = getSaveSlotFilePath();
			const auto storageClient = modules->storage->getClient(scope);
			storageClient->writeString(storagePath(StorePaths::GameData), *result->getResult<std::string>());
			hasSavedData = true;

			if (backOutToMainMenuAfterSave && hasFinishedTransition)
			{
				const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
				modules->storage->cleanupScopeAsync(storageScopeName);
				removeFromParent();
				applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
			}
			else
			{
				modules->storage->saveScopeAsync(storageScopeName, weak_from_this());
			}
		}
		break;

		case AW::WorkerTaskCode::STORE_SAVE_SCOPE:
			isSavingData = false;
			break;
		}
	}

	void SceneMainGame::onFadeOut()
	{
		this->modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
		hasFinishedTransition = true;
	}

	void SceneMainGame::onFadeIn()
	{
		globalParticleSystem->clear();

		if (backOutToMainMenuAfterSave && hasSavedData)
		{
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			removeFromParent();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
			hasFinishedTransition = true;
		}
		else if (!nextSceneName.empty())
		{
			masterSceneContainer->transitionToSceneWithBundle(nextSceneName, transitionBundle);
			nextSceneName = std::string();
			transitionBundle = AW::SceneTransitionBundle();
			globalTransition->fadeOut();
		}
		else
		{
			hasFinishedTransition = true;
		}
	}

	bool SceneMainGame::isTransitioning()
	{
		return hasFinishedTransition == false;
	}

	void SceneMainGame::transitionToScene(SceneGame sceneName)
	{
		transitionToScene(BaseScene::sceneToStr(sceneName));
	}

	void SceneMainGame::transitionToScene(std::string scene)
	{
		AW::SceneTransitionBundle bundle;
		transitionToSceneWithBundle(scene, bundle);
	}

	void SceneMainGame::transitionToSceneWithBundle(std::string scene, AW::SceneTransitionBundle& bundle)
	{
		hasFinishedTransition = false;

		this->modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 0.0);
		transitionBundle = bundle;
		nextSceneName = scene;
		globalTransition->fadeIn();
	}

	void SceneMainGame::backOutToMainMenu()
	{
		backOutToMainMenuAfterSave = true;
		hasFinishedTransition = false;

		this->modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 0.0);
		saveGameData();
		globalTransition->fadeIn();
	}

	void SceneMainGame::saveGameData()
	{
		if (isSavingData)
		{
			return;
		}

		modules->logger->log("SceneGame::Saving Game Data");
		hasSavedData = false;
		isSavingData = true;

		const auto baseScene = std::dynamic_pointer_cast<BaseScene>(masterSceneContainer->getCurrentScene());
		if (baseScene != nullptr)
		{
			baseScene->onAboutToSave();
		}

		this->modules->serialization->serializeAsync(this->shared_from_this(), weak_from_this());
	}

	void SceneMainGame::enableMenu()
	{
		menuEnabled = true;
	}

	void SceneMainGame::disableMenu()
	{
		menuEnabled = false;
	}

	void SceneMainGame::showOptions()
	{
		disableMenu();
		gameMainMenu->hide();
		masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::OptionsMenu));
	}

	void SceneMainGame::hideOptions()
	{
		masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Tetris));
		enableMenu();
		gameMainMenu->show();
	}

	std::shared_ptr<AW::SerializationClient> SceneMainGame::doSerialize(AW::SerializationHint hint)
	{
		if (!nextSceneName.empty())
		{
			masterSceneContainer->transitionToSceneWithBundle(nextSceneName, transitionBundle);
			nextSceneName = std::string();
			transitionBundle = AW::SceneTransitionBundle();
		}

		const auto client = serializationClient->getClient("scene_main_game", hint);
		menuEnabled = client->serializeBool("menu_enabled", menuEnabled);

		return BaseScene::doSerialize(hint);
	}

	void SceneMainGame::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_ESCAPE:
			if (!menuEnabled)
			{
				return;
			}

			if (gameMainMenu->visible)
			{
				gameMainMenu->hide();
			}
			else
			{
				gameMainMenu->show();
			}
			break;
		}
	}
}