#pragma once

#include <list>
#include <set>
#include <map>
#include <memory>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include <SDL_ttf.h>
#include "engine/module/IBaseModule.h"
#include "engine/module/input/Input.h"
#include "engine/module/thread/Thread.h"
#include "EnterFrameListener.h"
#include "ApplicationEvent.h"

namespace AW
{
	class Event : public IBaseModule
	{
		class EnterFrameListenerBundle
		{
		public:
			EnterFrameListenerBundle(std::shared_ptr<EnterFrameListener> ptr) : id(ptr->getObjectIdEnterFrame()), ptr(ptr) { }
			const int id;
			const std::weak_ptr<EnterFrameListener> ptr;
		};

		class TimeoutBundle
		{
		public:
			TimeoutBundle(int id, std::shared_ptr<EnterFrameListener> ptr, double time) : ptr(ptr), time(time), id(id) {}
			const int id = 0;
			const std::weak_ptr<EnterFrameListener> ptr;

			double time = 0.0;
		};

		std::shared_ptr<Input> input;
		std::shared_ptr<Thread> thread;

		int timeoutId = 100;
		bool processingOnEnterFrames = false;

		std::set<int> enterFrameObjects;
		std::map<int, std::list<std::shared_ptr<EnterFrameListenerBundle>>, std::greater<int>> onEnterFrameCallbacks;
		std::list<std::shared_ptr<EnterFrameListener>> cleanupObjects;
		std::list<std::shared_ptr<TimeoutBundle>> timeoutCallbacks;
		std::list<std::shared_ptr<TimeoutBundle>> timeoutProcessedCallbacks;
		std::list<int> timeoutProcessedRemoveCallbacks;
		std::list<std::shared_ptr<ApplicationEvent>> events;

		void reportSdlErrors();

	public:
		void bindInput(std::shared_ptr<Input> input);
		void bindThread(std::shared_ptr<Thread> thread);

		void pushEvent(std::shared_ptr<ApplicationEvent> event);
		std::list<std::shared_ptr<ApplicationEvent>> processEvents();

		void processEnterFrames(double frameTime);

		void registerOnEnterFrame(std::shared_ptr<EnterFrameListener> listener, int priority = 0);
		void unregisterOnEnterFrame(std::shared_ptr<EnterFrameListener> listener);

		int registerTimeoutCallback(std::shared_ptr<EnterFrameListener> listener, double timeoutMS);
		void unregisterTimeoutCallback(int id);

		void onCleanup();
	};
}
