#include "BaseScene.h"

#include "GameImports.h"

namespace
{
	const AWGame::Scenes config;

	const std::string gameStoragePrefix = "game-store-";
	const std::string gameFileStoragePrefix = "save-";
}

namespace AWGame
{
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

		std::make_unique<SceneTetris>();
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

	BaseScene::BaseScene(SceneGame name) : AWCore::Scene(config.game.at(name)) {}

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

	void BaseScene::transitionToSceneWithBundle(SceneGame scene, AWCore::SceneTransitionBundle & bundle)
	{
		transitionToSceneWithBundle(config.game.at(scene), bundle);
	}

	void BaseScene::transitionToSceneWithBundle(std::string sceneName, AWCore::SceneTransitionBundle& bundle)
	{
		const auto rootSceneContainer = getSceneContainer();

		if (rootSceneContainer != nullptr)
		{
			rootSceneContainer->transitionToSceneWithBundle(sceneName, bundle);
		}
	}

	std::weak_ptr<AWCore::ApplicationObject> BaseScene::baseSceneWeakThisRef()
	{
		return std::dynamic_pointer_cast<ApplicationObject>(shared_from_this());
	}
}
