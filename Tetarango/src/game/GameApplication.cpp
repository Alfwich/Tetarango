#include "GameApplication.h"

#include "scene/Scenes.h"
#include "scene/BaseScene.h"
#include "ui/scene/Scene.h"
#include "ui/scene/SceneContainer.h"

#include "scene/test/TestScene.h"
#include "scene/test/TestScene2.h"
#include "scene/test/tetris/SceneTetris.h"

#include "scene/menu/SceneSplash.h"
#include "scene/menu/SceneMainMenu.h"
#include "scene/menu/SceneOptionsMenu.h"
#include "scene/menu/SceneSavedGamesMenu.h"

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
	const auto displayDebugPhysicParamKey = "display-debug-physic-enabled";

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
			screenConfig.useDoubleBuffer = storageClient->readBool(displayDoubleBufferParamKey);
			screenConfig.openGlWireframeMode = storageClient->readBool(displayWireframeModeParamKey);
			screenConfig.frameLimiter = storageClient->readInt(displayFrameLimiterParamKey);

			screenConfig.visualizeClipRects = storageClient->readBool(displayDebugClipRectsParamKey);
			screenConfig.visualizeContainers = storageClient->readBool(displayDebugContainersParamKey);
			screenConfig.debugOverlayEnabled = storageClient->readBool(displayDebugOverlayParamKey);
			screenConfig.visualizePhysic = storageClient->readBool(displayDebugPhysicParamKey);
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

		screenBuffer = std::make_shared<AW::DisplayBuffer>();
		screenBuffer->setFragmentShader(modules->shader->getShader({ "f-cached", "f-texture", "f-color" }));
		screenBuffer->renderUpdateMode = AW::RenderUpdateMode::EveryFrame;
		screenBuffer->renderPositionMode = AW::RenderPositionMode::AbsoluteSelfOnly;
		screenBuffer->setClearColor(0, 0, 0);
		screenBuffer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		screenBuffer->topLeftAlignSelf();
		root->add(screenBuffer);

		const auto storageClient = modules->storage->getClient();

		if (gameConfig->getConfigBool(Config::Param::hydrateOnLoad) && storageClient->hasKey("whole_scene_graph"))
		{
			masterSceneContainer = std::static_pointer_cast<AW::SceneContainer>(modules->serialization->hydrate(storageClient->readSring("whole_scene_graph")));
			screenBuffer->add(masterSceneContainer);

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
			screenBuffer->add(masterSceneContainer);

			masterSceneContainer->add(std::make_shared<SceneSplash>());
			masterSceneContainer->add(std::make_shared<SceneMainMenu>());
			masterSceneContainer->add(std::make_shared<SceneOptionsMenu>());
			masterSceneContainer->add(std::make_shared<SceneSavedGamesMenu>());

			if (gameConfig->getConfigBool(Config::Param::enableTestScenes))
			{
				masterSceneContainer->add(std::make_shared<TestScene>());
				masterSceneContainer->add(std::make_shared<TestScene2>());
				masterSceneContainer->add(std::make_shared<SceneTetris>());
			}

			masterSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::Splash));
		}

		debugMonitor = std::make_shared<AW::DebugMonitor>();
		debugMonitor->zIndex = 1;
		debugMonitor->setScreenPosition(5.0, 5.0);
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
		storageClient->writeBool(displayDoubleBufferParamKey, screenConfig.useDoubleBuffer);

		storageClient->writeBool(displayWireframeModeParamKey, screenConfig.openGlWireframeMode);
		storageClient->writeBool(displayDebugClipRectsParamKey, screenConfig.visualizeClipRects);
		storageClient->writeBool(displayDebugContainersParamKey, screenConfig.visualizeContainers);
		storageClient->writeBool(displayDebugOverlayParamKey, screenConfig.debugOverlayEnabled);
		storageClient->writeBool(displayDebugPhysicParamKey, screenConfig.visualizePhysic);

		if (screenBuffer != nullptr)
		{
			screenBuffer->rebuildInternalTexture();
			screenBuffer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
			screenBuffer->topLeftAlignSelf();
		}

		if (masterSceneContainer != nullptr)
		{
			masterSceneContainer->onDisplayProvisioned();
		}

		if (debugMonitor != nullptr)
		{
			debugMonitor->clear();
			debugMonitor->visible = screenConfig.debugOverlayEnabled;
			debugMonitor->enterFrameActivated = screenConfig.debugOverlayEnabled;
		}
	}
}
