#include "Application.h"

#include "serialization-list.generated.h"
#include "Tests.h"

namespace
{
	AW::GeneratedPrimeList primeList;
}

namespace AW
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

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
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
		modules->asset->onInit();
		provisionScreen();

		modules->logger->log("Application::Init modules");
		modules->onInit();

		frameTimer = modules->time->createTimer(AW::TimeScope::ApplicationFrameTimer, true);

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
		modules->logger->log("Application::Modules load resources");
		modules->onLoadResources();

		modules->logger->log("Application::Priming system serialization pool and loading resources");
		primeSerializationAndLoadResources();

		modules->logger->log("Application::onLoadResources");
		onLoadResources();

		modules->logger->log("Application::onPrimeSerialization");
		onPrimeSerialization();

		if (gameConfig->getConfigBool(Config::Param::runTests))
		{
			AW::Logger::instance()->logCritical("Tests::Running tests");
			AWTest::Tests::run(modules);
			AW::Logger::instance()->logCritical("Tests::Done.");
		}

		modules->logger->log("Application::Ready modules");
		modules->onReady();

		running = true;

		modules->logger->log("Application::Preparing display root");
		root->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		root->onInitialAttach();
		root->createChildren();

		modules->logger->log("Application::onReady");
		onReady();

		frameTimer->start();
	}

	void Application::primeSerializationAndLoadResources()
	{
		primeList.prime();
	}

	void Application::run(int argc, char* args[])
	{
		init();
		ready();
		while (running)
		{
			updateFrameTime();
			processEnterFrames();
			processApplicationEvents();
			render();
			doFrameLimitIfNeeded();
		}
		cleanup();

		modules->logger->log("Application::Run end\n");
	}

	void Application::cleanup()
	{
		modules->logger->log("Application::onCleanup");
		onCleanup();

		modules->logger->log("Application::Cleanup modules");
		modules->onCleanup();

		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void Application::updateFrameTime()
	{
		frameTime = std::min(100.0, frameTimer->getTicksAndRestart());
		startFrameTime = modules->time->getHighResolutionTicks();
	}

	void Application::processEnterFrames()
	{
		modules->onEnterFrame(frameTime);
	}

	void Application::processApplicationEvents()
	{
		for (auto e : modules->event->getEvents())
		{
			switch (e->code)
			{
			case AW::Events::QuitRequested:
				exit();
				break;

			case AW::Events::ReprovisionScreen:
				const auto reprovisionScreenEvent = std::static_pointer_cast<AW::ReprovisionScreenApplicationEvent>(e);
				if (e != nullptr)
				{
					screenConfig = reprovisionScreenEvent->config;
					provisionScreen();
				}
				break;
			}
		}
	}

	void Application::doFrameLimitIfNeeded()
	{
		if (screenConfig.frameLimiter > 0)
		{
			const auto timeDiffToDelay = targetFrameTime - (modules->time->getHighResolutionTicks() - startFrameTime) - 1.0;
			if (timeDiffToDelay > 0.0)
			{
				modules->time->delay((Uint32)std::floor(timeDiffToDelay));
			}

			// Spinlock the rest of the time
			modules->time->spinlock(startFrameTime, targetFrameTime);
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
		const auto isFullscreen = screenConfig.mode == ScreenModes::Fullscreen || screenConfig.mode == ScreenModes::FullscreenDesktop;
		Logger::instance()->log("Application::Provisioning screen width=" + std::to_string(screenConfig.width) + ", height=" + std::to_string(screenConfig.height) + ", fullscreen=" + std::to_string(isFullscreen));
		if (modules->screen->init(screenConfig, gameConfig->getConfigString(Config::Param::gameName)))
		{
			if (screenConfig.frameLimiter > 0)
			{
				targetFrameTime = (1000.0 / screenConfig.frameLimiter);
			}

			if (root != nullptr)
			{
				root->setSize(modules->screen->getWidth(), modules->screen->getHeight());
			}

			onProvisionedScreen();
		}
		else if (onFailedToProvisionScreen())
		{
			Logger::instance()->logCritical("Application::Provisioning screen failed. Falling back to current display resolution or safe fallback");
			provisionScreen();
		}
	}
}
