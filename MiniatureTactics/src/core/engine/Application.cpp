#include "Application.h"

#include "engine/ApplicationObject.h"
#include "ui/Camera.h"
#include "ui/scene/Scene.h"
#include "ui/scene/SceneContainer.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/Rectangle.h"
#include "ui/renderable/element/Backdrop.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/container/ScrollContainer.h"
#include "ui/renderable/Zone.h"
#include "ui/renderable/primitive/trace/Trace.h"
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/element/CachedImage.h"

#include "Tests.h"

namespace MT
{
	Application::Application()
	{
		gameConfig = std::make_shared<Config::GameConfig>();
		screenConfig.width = 640;
		screenConfig.height = 480;
		screenConfig.windowFlags = SDL_WINDOW_SHOWN;
	}

	bool Application::init()
	{
		modules = std::make_shared<SystemModuleBundle>();
		modules->logger->log("Application::Run");

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) != 0)
		{
			modules->logger->logFatal("Application::SDL::Failed to init: " + std::string(SDL_GetError()));
		}

		if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
		{
			modules->logger->logFatal("Application::SDL_IMG::Failed to init: " + std::string(IMG_GetError()));
		}

		if (TTF_Init() == -1)
		{
			modules->logger->logFatal("Application::SDL_TTF::Failed to init: " + std::string(TTF_GetError()));
		}

		if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3)
		{
			modules->logger->logFatal("Application::SDL_MIX::Failed to init: " + std::string(Mix_GetError()));
		}

		modules->logger->log("Application::Assign GameConfig");
		modules->onAssignConfig(gameConfig);

		modules->logger->log("Application::onInit");
		onInit();
		provisionScreen();

		modules->logger->log("Application::Init modules");
		modules->onInit();

		frameTimer = modules->time->createTimer(MT::TimeScope::ApplicationFrameTimer, true);
		root = std::make_shared<DisplayRoot>();

		return true;
	}

	bool Application::isRunning()
	{
		return running;
	}

	bool Application::hasInit()
	{
		return frameTimer != nullptr;
	}

	void Application::ready()
	{
		modules->logger->log("Application::Load resources");
		loadResources();

		modules->logger->log("Application::Priming system serialization pool");
		primeSerialization();

		modules->logger->log("Application::onLoadResources");
		onLoadResources();

		modules->logger->log("Application::onPrimeSerialization");
		onPrimeSerialization();

		if (gameConfig->getConfigBool(Config::Param::runTests))
		{
			MT::Logger::instance()->logCritical("Tests::Running tests");
			MTTest::Tests::run(modules);
			MT::Logger::instance()->logCritical("Tests::Done.");
		}

		modules->logger->log("Application::Ready modules");
		modules->onReady();

		running = true;

		modules->logger->log("Application::onReady");
		onReady();

		frameTimer->start();
	}

	void Application::loadResources()
	{
		modules->texture->loadTexture("res/core/img/solid.png", "__solid__");
	}

	void Application::primeSerialization()
	{
		std::make_unique<ApplicationObject>();
		std::make_unique<Rectangle>();
		std::make_unique<Scene>();
		std::make_unique<SceneContainer>();
		std::make_unique<Text>();
		std::make_unique<ParticleSystem>();
		std::make_unique<TileMap>();
		std::make_unique<Container>();
		std::make_unique<ScrollContainer>();
		std::make_unique<Backdrop>();
		std::make_unique<Zone>();
		std::make_unique<Animated>();
		std::make_unique<Element>();
		std::make_unique<Trace>();
		std::make_unique<NineSlice>();
		std::make_unique<CachedImage>();
	}

	void Application::run(int argc, char* args[])
	{
		init();
		ready();
		while (running)
		{
			double frameTime = std::min(100.0, frameTimer->getTicksAndRestart());
			processApplicationEvents();
			modules->event->processEnterFrames(frameTime);
			render();
			modules->event->processPostRenderCallbacks();
			modules->collision->processCollisions();
		}
		cleanup();

		modules->logger->log("Application::Run end\n");
	}

	void Application::cleanup()
	{
		modules->logger->log("Application::onCleanup");
		onCleanup();

		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

		modules->logger->log("Application::Cleanup modules");
		modules->onCleanup();

	}

	void Application::processApplicationEvents()
	{
		for (auto e : modules->event->processEvents())
		{
			switch (e->code)
			{
			case MT::Events::QUIT_REQUESTED:
				exit();
				break;

			case MT::Events::CHANGE_RESOLUTION:
				screenConfig.width = MT::StringHelper::getDisplayComponentForDisplayString(&e->what, 0);
				screenConfig.height = MT::StringHelper::getDisplayComponentForDisplayString(&e->what, 1);
				provisionScreen();
			default:
				break;
			}
		}
	}

	void Application::exit()
	{
		running = false;
	}

	void Application::render()
	{
		modules->screen->render(root);
	}

	void Application::provisionScreen()
	{
		Logger::instance()->log("Application::Provisioning screen width=" + std::to_string(screenConfig.width) + ", height= " + std::to_string(screenConfig.height) + ", fullscreen=" + std::to_string(screenConfig.isFullscreen));
		if (modules->screen->init(screenConfig, gameConfig->getConfigString(Config::Param::gameName)))
		{
			modules->texture->rebindAllTextures();
			onProvisionedScreen();
		}
		else if (onFailedToProvisionScreen())
		{
			Logger::instance()->logCritical("Application::Provisioning screen failed. Falling back to current display resolution or safe fallback");
			provisionScreen();
		}
	}
}
