#include "BaseScene.h"

#include "GameImports.h"

namespace
{
	const MTGame::Scenes config;

	const std::string gameStoragePrefix = "game-store-";
	const std::string gameFileStoragePrefix = "save-";
}

namespace MTGame
{
	bool BaseScene::isLoaded = false;

	const Scenes& BaseScene::getScenes()
	{
		return config;
	}

	void BaseScene::primeScenes()
	{
		std::make_unique<TestScene>();
		std::make_unique<TestSpaceScene>();

		std::make_unique<SceneSplash>();
		std::make_unique<SceneMainMenu>();
		std::make_unique<SceneOptionsMenu>();
		std::make_unique<SceneSavedGamesMenu>();
		std::make_unique<SceneMainGame>();
	}

	void BaseScene::loadResources(std::shared_ptr<MT::SystemModuleBundle> modules)
	{
		if (isLoaded)
		{
			return;
		}

		modules->sound->loadSoundClip("res/game/snd/medium.wav", "test");
		modules->texture->loadTexture("res/game/img/tile/grass.png", "tile-grass");
		modules->texture->loadTexture("res/game/img/tile/wood-floor.png", "tile-wood-floor");
		modules->texture->loadTexture("res/game/img/tile/test-scene-background.png", "test-scene-background");

		isLoaded = true;
	}

	std::string BaseScene::sceneToStr(SceneGame scene)
	{
		return config.game.at(scene);
	}

	std::string BaseScene::storagePath(StorePaths path)
	{
		return gameStoragePrefix + std::to_string((int)path);
	}

	std::string BaseScene::filePath(StorePaths path)
	{
		switch (path)
		{
		case StorePaths::System_SaveSlot1:
			return gameFileStoragePrefix + '1';
			break;

		case StorePaths::System_SaveSlot2:
			return gameFileStoragePrefix + '2';
			break;

		case StorePaths::System_SaveSlot3:
			return gameFileStoragePrefix + '3';
			break;
		}

		return gameFileStoragePrefix + std::to_string((int)path);
	}

	BaseScene::BaseScene(SceneGame name) : MT::Scene(config.game.at(name)) {}

	int BaseScene::getScreenWidth()
	{
		return modules->screen->getWidth();
	}

	int BaseScene::getScreenHeight()
	{
		return modules->screen->getHeight();
	}

	void BaseScene::transitionToScene(SceneGame scene)
	{
		transitionToScene(config.game.at(scene));
	}

	void BaseScene::transitionToScene(std::string sceneName)
	{
		const auto rootSceneContainer = getSceneContainer();

		if (rootSceneContainer != nullptr)
		{
			rootSceneContainer->transitionToScene(sceneName);
		}
	}

	void BaseScene::transitionToSceneWithBundle(SceneGame scene, MT::SceneTransitionBundle & bundle)
	{
		transitionToSceneWithBundle(config.game.at(scene), bundle);
	}

	void BaseScene::transitionToSceneWithBundle(std::string sceneName, MT::SceneTransitionBundle& bundle)
	{
		const auto rootSceneContainer = getSceneContainer();

		if (rootSceneContainer != nullptr)
		{
			rootSceneContainer->transitionToSceneWithBundle(sceneName, bundle);
		}
	}

	std::weak_ptr<MT::ApplicationObject> BaseScene::baseSceneWeakThisRef()
	{
		return std::dynamic_pointer_cast<ApplicationObject>(shared_from_this());
	}
}
