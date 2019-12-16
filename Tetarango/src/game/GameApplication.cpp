#include "GameApplication.h"
#include "GameImports.h"

namespace
{
	const auto displayWidthParamKey = "display-width";
	const auto displayHeightParamKey = "display-height";
	const auto displayModeParamKey = "display-fullscreen";
	const auto displayVsyncModeParamKey = "display-v-sync-mode";
	const auto displayMSAAParamKey = "display-MSAA";
	const auto displayOpenGLCompatibilityParamKey = "display-open-gl-compadibility";
	const auto displayWireframeModeParamKey = "display-wireframe-mode";
	const auto displayDoubleBufferParamKey = "display-double-buffer";
	const auto displayFrameLimiterParamKey = "display-frame-limiter";

	const auto masterVolParamKey = "vol-master";
	const auto generalVolParamKey = "vol-general";
	const auto musicVolParamKey = "vol-music";
}

namespace AWGame
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
			screenConfig.mode = (AWCore::ScreenModes)storageClient->readInt(displayModeParamKey);
			screenConfig.vMode = (AWCore::VsyncModes)storageClient->readInt(displayVsyncModeParamKey);
			screenConfig.msaaSamples = storageClient->readInt(displayMSAAParamKey);
			screenConfig.openGLCompatibilityMode = storageClient->readBool(displayOpenGLCompatibilityParamKey);
			screenConfig.openGlWireframeMode = storageClient->readBool(displayWireframeModeParamKey);
			screenConfig.useDoubleBuffer = storageClient->readBool(displayDoubleBufferParamKey);
			screenConfig.frameLimiter = storageClient->readInt(displayFrameLimiterParamKey);
		}

		screenConfig.windowFlags = SDL_WINDOW_SHOWN;
	}

	void GameApplication::onPrimeSerialization()
	{
		BaseGui::primeGuis();
		BaseScene::primeScenes();
		BaseProp::primeProps();
		std::make_unique<Board>();
	}

	void GameApplication::onReady()
	{
		modules->screen->setClearColor(0, 0, 0);

		modules->time->createTimeScope(AWCore::TimeScope::Game, 0.0);
		modules->time->createTimeScope(AWCore::TimeScope::Menu, 1.0);
		modules->time->createTimeScope(AWCore::TimeScope::Camera, 1.0);

		const auto storageClient = modules->storage->getClient();

		if (gameConfig->getConfigBool(Config::Param::hydrateOnLoad) && storageClient->hasKey("whole_scene_graph"))
		{
			masterSceneContainer = std::static_pointer_cast<AWCore::SceneContainer>(modules->serialization->hydrate(storageClient->readSring("whole_scene_graph")));
			root->add(masterSceneContainer);

			if (!gameConfig->getConfigBool(Config::Param::launchToLastScene))
			{
				masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Splash));
			}
		}
		else
		{
			masterSceneContainer = std::make_shared<AWCore::SceneContainer>();
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

		if (storageClient->hasKey(masterVolParamKey))
		{
			modules->sound->setMasterVolume(storageClient->readDouble(masterVolParamKey));
			modules->sound->setEffectVolume(storageClient->readDouble(generalVolParamKey));
			modules->sound->setMusicVolume(storageClient->readDouble(musicVolParamKey));
		}
		else
		{
			modules->sound->setMasterVolume(1.0);
			modules->sound->setEffectVolume(0.8);
			modules->sound->setMusicVolume(0.6);
		}
	}

	void GameApplication::onCleanup()
	{
		const auto storageClient = modules->storage->getClient();

		storageClient->writeDouble(masterVolParamKey, modules->sound->getMasterVolume());
		storageClient->writeDouble(generalVolParamKey, modules->sound->getEffectVolume());
		storageClient->writeDouble(musicVolParamKey, modules->sound->getMusicVolume());

		if (gameConfig->getConfigBool(Config::Param::saveWholeSceneOnClose))
		{
			storageClient->writeString("whole_scene_graph", modules->serialization->serialize(masterSceneContainer));
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
			screenConfig.mode = AWCore::ScreenModes::Windowed;
		}
		else
		{
			screenConfig.width = width;
			screenConfig.height = height;
			screenConfig.mode = AWCore::ScreenModes::Windowed;
		}

		return true;
	}

	void GameApplication::onProvisionedScreen()
	{
		const auto storageClient = modules->storage->getClient();
		storageClient->writeInt(displayWidthParamKey, screenConfig.width);
		storageClient->writeInt(displayHeightParamKey, screenConfig.height);
		storageClient->writeInt(displayModeParamKey, (int)screenConfig.mode);
		storageClient->writeInt(displayVsyncModeParamKey, (int)screenConfig.vMode);
		storageClient->writeInt(displayMSAAParamKey, screenConfig.msaaSamples);
		storageClient->writeInt(displayFrameLimiterParamKey, screenConfig.frameLimiter);
		storageClient->writeBool(displayOpenGLCompatibilityParamKey, screenConfig.openGLCompatibilityMode);
		storageClient->writeBool(displayWireframeModeParamKey, screenConfig.openGlWireframeMode);

		if (masterSceneContainer != nullptr)
		{
			masterSceneContainer->onDisplayProvisioned();
		}
	}
}
