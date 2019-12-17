#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameConfig.h"
#include "engine/module/time/Timer.h"
#include "engine/module/SystemModuleBundle.h"
#include "engine/module/display/ScreenConfig.h"

namespace AW
{
	class DisplayRoot;

	class Application : public IInputListener, public INotifyOnCompletion, public std::enable_shared_from_this<Application>
	{
	private:
		std::shared_ptr<Timer> frameTimer;
		double startFrameTime = 0.0, targetFrameTime = 0.0, frameTime = 0.0;
		bool running = false;

		void updateFrameTime();
		void processEnterFrames();
		void processApplicationEvents();
		void processCollisions();
		void doFrameLimitIfNeeded();

	protected:
		ScreenConfig screenConfig;
		std::shared_ptr<DisplayRoot> root;

	public:
		Application();

		std::shared_ptr<Config::GameConfig> gameConfig;

		bool init();
		void loadResources();
		void primeSerialization();
		void ready();
		void cleanup();
		void exit();

		virtual void onWorkError(WORKER_ID workerId, WorkerTaskCode code) { /* NO-OP */ };
		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<AsyncResultBundle> result) {
			Logger::instance()->logCritical("Application::Worker returned data but onWorkDone was not overridden.");
		}
		virtual void onInit() { /* NO-OP */ };
		virtual void onLoadResources() { /* NO-OP */ };
		virtual void onPrimeSerialization() { /* NO-OP */ };
		virtual void onReady() { /* NO-OP */ };
		virtual void onCleanup() { /* NO-OP */ };

		void run(int argc, char* args[]);
		bool isRunning();
		bool hasInit();

		std::shared_ptr<SystemModuleBundle> modules;
		virtual void render();

		virtual bool onFailedToProvisionScreen() { return false; };
		virtual void onProvisionedScreen() { /* NO-OP */ };
		void provisionScreen();
	};
}

#include "engine/module/display/DisplayRoot.h"
