#include "GameApplication.h"

#include "GameImports.h"

namespace MTGame
{
	void GameApplication::onInit()
	{
		int defaultScreenWidth = 1024, defaultScreenHeight = 768;
		for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {

			SDL_DisplayMode current;

			if (SDL_GetCurrentDisplayMode(i, &current) == 0)
			{
				defaultScreenWidth = current.w;
				defaultScreenHeight = current.h;
				break;
			}
			else
			{
				modules->logger->logCritical("GameApplication::Could not get display mode for video display: " + std::string(SDL_GetError()));
			}
		}

		screenConfig.width = defaultScreenWidth;
		screenConfig.height = defaultScreenHeight;
		screenConfig.windowFlags = SDL_WINDOW_SHOWN;
		//screenConfig.isFullscreen = true;
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

}
