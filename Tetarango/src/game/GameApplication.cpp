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
	const auto displayDebugClipRectsParamKey = "display-debug-clip-rects-buffer";
	const auto displayDebugContainersParamKey = "display-debug-containers-limiter";
	const auto displayDebugOverlayParamKey = "display-debug-overlay-enabled";

	const auto masterVolParamKey = "vol-master";
	const auto generalVolParamKey = "vol-general";
	const auto musicVolParamKey = "vol-music";

	const auto scenes = AWGame::Scenes();
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
			screenConfig.mode = (AW::ScreenModes)storageClient->readInt(displayModeParamKey);
			screenConfig.vMode = (AW::VsyncModes)storageClient->readInt(displayVsyncModeParamKey);
			screenConfig.msaaSamples = storageClient->readInt(displayMSAAParamKey);
			screenConfig.openGLCompatibilityMode = storageClient->readBool(displayOpenGLCompatibilityParamKey);
			screenConfig.openGlWireframeMode = storageClient->readBool(displayWireframeModeParamKey);
			screenConfig.useDoubleBuffer = storageClient->readBool(displayDoubleBufferParamKey);
			screenConfig.frameLimiter = storageClient->readInt(displayFrameLimiterParamKey);
			screenConfig.visualizeClipRects = storageClient->readBool(displayDebugClipRectsParamKey);
			screenConfig.visualizeContainers = storageClient->readBool(displayDebugContainersParamKey);
			screenConfig.debugOverlayEnabled = storageClient->readBool(displayDebugOverlayParamKey);
		}

		screenConfig.windowFlags = SDL_WINDOW_SHOWN;

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

	void GameApplication::onPrimeSerialization()
	{
		BaseGui::primeGuis();
		BaseScene::primeScenes();
		BaseProp::primeProps();
		std::make_unique<Board>();
	}

	void GameApplication::onLoadResources()
	{
		modules->font->loadFont("res/font/Roboto-Medium.ttf", "medium");
		modules->font->loadFont("res/font/fixedsys.ttf", "console");
	}

	void GameApplication::onReady()
	{
		modules->screen->setClearColor(0, 0, 0);

		modules->time->createTimeScope(AW::TimeScope::Game, 0.0);
		modules->time->createTimeScope(AW::TimeScope::Menu, 1.0);
		modules->time->createTimeScope(AW::TimeScope::Camera, 1.0);

		const auto storageClient = modules->storage->getClient();

		if (gameConfig->getConfigBool(Config::Param::hydrateOnLoad) && storageClient->hasKey("whole_scene_graph"))
		{
			masterSceneContainer = std::static_pointer_cast<AW::SceneContainer>(modules->serialization->hydrate(storageClient->readSring("whole_scene_graph")));
			root->add(masterSceneContainer);

			if (!gameConfig->getConfigBool(Config::Param::launchToLastScene))
			{
				masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Splash));

				if (masterSceneContainer->hasScene(scenes.game.at(SceneGame::MainGame)))
				{
					masterSceneContainer->removeScene(scenes.game.at(SceneGame::MainGame));
				}
			}
		}
		else
		{
			masterSceneContainer = std::make_shared<AW::SceneContainer>();
			root->add(masterSceneContainer);

			masterSceneContainer->add(std::make_shared<SceneSplash>());
			masterSceneContainer->add(std::make_shared<SceneMainMenu>());
			masterSceneContainer->add(std::make_shared<SceneOptionsMenu>());
			masterSceneContainer->add(std::make_shared<SceneSavedGamesMenu>());

			if (gameConfig->getConfigBool(Config::Param::enableTestScenes))
			{
				masterSceneContainer->add(std::make_shared<TestScene>());
				masterSceneContainer->add(std::make_shared<TestSpace2>());
			}

			masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Splash));
		}

		debugMonitor = std::make_shared<AW::DebugMonitor>();
		debugMonitor->zIndex = 1;
		debugMonitor->setPosition(5.0, 5.0);
		if (!screenConfig.debugOverlayEnabled)
		{
			debugMonitor->visible = false;
			debugMonitor->enterFrameActivated = false;
		}
		root->add(debugMonitor);
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
			screenConfig.mode = AW::ScreenModes::Windowed;
		}
		else
		{
			screenConfig.width = width;
			screenConfig.height = height;
			screenConfig.mode = AW::ScreenModes::Windowed;
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
		storageClient->writeBool(displayDebugClipRectsParamKey, screenConfig.visualizeClipRects);
		storageClient->writeBool(displayDebugContainersParamKey, screenConfig.visualizeContainers);
		storageClient->writeBool(displayDebugOverlayParamKey, screenConfig.debugOverlayEnabled);

		if (masterSceneContainer != nullptr)
		{
			masterSceneContainer->onDisplayProvisioned();
		}

		if (debugMonitor != nullptr)
		{
			if (screenConfig.debugOverlayEnabled)
			{
				debugMonitor->clear();
				debugMonitor->visible = true;
				debugMonitor->enterFrameActivated = true;
			}
			else
			{
				debugMonitor->visible = false;
				debugMonitor->enterFrameActivated = false;
			}
		}
	}
}
