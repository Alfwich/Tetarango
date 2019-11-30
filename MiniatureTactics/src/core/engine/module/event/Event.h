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

namespace MT
{
	class Event : public IBaseModule
	{
		class EnterFrameListenerBundle
		{
		public:
			EnterFrameListenerBundle(std::shared_ptr<EnterFrameListener> ptr) { this->id = ptr->getObjectIdEnterFrame(); this->ptr = ptr; }

			int id;
			std::weak_ptr<EnterFrameListener> ptr;
		};

		std::shared_ptr<Input> input;
		std::shared_ptr<Thread> thread;

		bool processingOnEnterFrames = false, processingOnPostRenders = false;

		std::set<int> enterFrameObjects;
		std::map<int, std::list<std::shared_ptr<EnterFrameListenerBundle>>, std::greater<int>> onEnterFrameCallbacks;
		std::list<std::shared_ptr<EnterFrameListener>> cleanupObjects;
		std::list<std::weak_ptr<EnterFrameListener>> postRenderCallbacks;
		std::list<std::weak_ptr<EnterFrameListener>> postRenderProcessedCallbacks;
		std::list<std::shared_ptr<ApplicationEvent>> events;

		void reportSdlErrors();

	public:
		Event();
		virtual ~Event();

		void bindInput(std::shared_ptr<Input> input);
		void bindThread(std::shared_ptr<Thread> thread);

		void pushEvent(std::shared_ptr<ApplicationEvent> event);
		std::list<std::shared_ptr<ApplicationEvent>> processEvents();

		void processEnterFrames(double frameTime);
		void processPostRenderCallbacks();
		void registerOnEnterFrame(std::shared_ptr<EnterFrameListener> listener, int priority = 0);
		void unregisterOnEnterFrame(std::shared_ptr<EnterFrameListener> listener);

		void registerPostRenderCallback(std::shared_ptr<EnterFrameListener> listener);
	};
}
