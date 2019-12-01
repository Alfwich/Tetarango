#include "GameApplication.h"
#include "GameImports.h"

namespace
{
	const auto displayWidthParamKey = "display-width";
	const auto displayHeightParamKey = "display-height";
	const auto displayFullscreenParamKey = "display-fullscreen";
}

namespace MTGame
{
	void GameApplication::onInit()
	{
		const auto storageClient = modules->storage->getClient();
		if (!storageClient->hasKey(displayWidthParamKey))
		{
			onFailedToProvisionScreen();
		}
		else
		{
			screenConfig.width = storageClient->readInt(displayWidthParamKey);
			screenConfig.height = storageClient->readInt(displayHeightParamKey);
			screenConfig.isFullscreen = storageClient->readBool(displayFullscreenParamKey);
		}

		screenConfig.windowFlags = SDL_WINDOW_SHOWN;
		//screenConfig.openGlWireframeMode = true;
	}

	void GameApplication::onLoadResources()
	{
		BaseGui::loadResources(modules);
		BaseScene::loadResources(modules);
		BaseProp::loadResources(modules);
		Board::loadResources(modules);
	}

	void GameApplication::onPrimeSerialization()
	{
		BaseGui::primeGuis();
		BaseScene::primeScenes();
		BaseProp::primeProps();
		Board::primeBoards();
	}

	void GameApplication::onReady()
	{
		modules->screen->setClearColor(0, 0, 0);

		modules->time->createTimeScope(MT::TimeScope::Game, 0.0);
		modules->time->createTimeScope(MT::TimeScope::Menu, 1.0);
		modules->time->createTimeScope(MT::TimeScope::Camera, 1.0);

		const auto globalStorageClient = modules->storage->getClient();

		if (gameConfig->getConfigBool(Config::Param::hydrateOnLoad) && globalStorageClient->hasKey("whole_scene_graph"))
		{
			masterSceneContainer = std::static_pointer_cast<MT::SceneContainer>(modules->serialization->hydrate(globalStorageClient->readSring("whole_scene_graph")));
			root->add(masterSceneContainer);

			if (!gameConfig->getConfigBool(Config::Param::launchToLastScene))
			{
				masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Splash));
			}
		}
		else
		{
			masterSceneContainer = std::make_shared<MT::SceneContainer>();
			root->add(masterSceneContainer);

			masterSceneContainer->add(std::make_shared<SceneSplash>());
			masterSceneContainer->add(std::make_shared<SceneMainMenu>());
			masterSceneContainer->add(std::make_shared<SceneOptionsMenu>());
			masterSceneContainer->add(std::make_shared<SceneSavedGamesMenu>());

			if (gameConfig->getConfigBool(Config::Param::enableTestScenes))
			{
				masterSceneContainer->add(std::make_shared<TestScene>());
				masterSceneContainer->add(std::make_shared<TestSpaceScene>());
			}

			masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Splash));
		}
	}

	void GameApplication::onCleanup()
	{
		if (gameConfig->getConfigBool(Config::Param::saveWholeSceneOnClose))
		{
			const auto globalClient = modules->storage->getClient();
			globalClient->writeString("whole_scene_graph", modules->serialization->serialize(masterSceneContainer));
			masterSceneContainer->disableCurrentScene();
		}
	}

	bool GameApplication::onFailedToProvisionScreen()
	{
		auto currentDisplayMode = modules->screen->getCurrentDisplayMode();
		int width, height;

		for (const auto mode : currentDisplayMode.modes)
		{
			if (mode.format != SDL_PIXELTYPE_UNKNOWN)
			{
				width = mode.w;
				height = mode.h;
			}
		}

		if (screenConfig.width == width && screenConfig.height == height)
		{
			screenConfig.width = 1024;
			screenConfig.height = 768;
			screenConfig.isFullscreen = false;
		}
		else
		{
			screenConfig.width = width;
			screenConfig.height = height;
			screenConfig.isFullscreen = false;
		}

		return true;
	}

	void GameApplication::onProvisionedScreen()
	{
		const auto storageClient = modules->storage->getClient();
		storageClient->writeInt(displayWidthParamKey, screenConfig.width);
		storageClient->writeInt(displayHeightParamKey, screenConfig.height);
		storageClient->writeBool(displayFullscreenParamKey, screenConfig.isFullscreen);

		if (masterSceneContainer != nullptr)
		{
			masterSceneContainer->onDisplayProvisioned();
		}
	}
}
