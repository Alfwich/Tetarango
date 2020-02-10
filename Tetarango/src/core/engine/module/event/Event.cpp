#include "Event.h"
#include "GL/glew.h"

namespace
{
	const auto eventExecureTimeoutFunctionName = "AW_exe_timeout";
	const auto eventSetTimeoutFunctionName = "setTimeout";
	const auto eventClearTimeoutFunctionName = "clearTimeout";
	const auto eventRegisterOnEventFrameFunctionName = "registerOnEnterFrame";
	const auto eventUnregisterOnEnterFrameFunctionName = "unregisterOnEnterFrame";
}

namespace AW
{

	void Event::bindInput(std::shared_ptr<Input> input)
	{
		this->input = input;
	}

	void Event::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	void Event::bindLua(std::shared_ptr<Lua> lua)
	{
		this->lua = lua;
	}

	void Event::pushEvent(std::shared_ptr<ApplicationEvent> event)
	{
		events.push_back(event);
	}

	const std::list<std::shared_ptr<ApplicationEvent>>& Event::getEvents()
	{
		return events;
	}

	void Event::processEvents()
	{
		events.clear();
		input->keyboard->purgeWeakRefs();
		input->mouse->purgeWeakRefs();
		input->gamepad->purgeWeakRefs();

		SDL_Event sldE;
		while (SDL_PollEvent(&sldE) != 0)
		{
			switch (sldE.type)
			{
			case SDL_QUIT:
				events.push_back(std::make_shared<ApplicationEvent>(Events::QuitRequested));
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				input->keyboard->updateKeyStates(&sldE);
				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				input->mouse->updateMouseState(&sldE);
				break;

			case SDL_JOYAXISMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_JOYHATMOTION:
				input->gamepad->updateGamepadState(&sldE);
				break;

			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
				input->gamepad->updateGamepadConnectedState(&sldE);
				break;
			}
		}

		thread->processWorkers();

#if _DEBUG
		if (gameConfig->getConfigBool(Config::Param::immediateDebugOutput))
		{
			reportSdlErrors();
		}
#endif // _DEBUG
	}

	int Event::setLuaTimeout(int luaId, double timeoutMS)
	{
		const auto bundle = std::make_shared<TimeoutBundle>(timeoutId++, luaId, timeoutMS);

		if (processingOnEnterFrames)
		{
			timeoutProcessedCallbacks.push_back(bundle);
		}
		else
		{
			timeoutCallbacks.push_back(bundle);
		}

		return bundle->id;
	}

	void Event::clearLuaTimeout(int luaId)
	{
		if (luaBindingIdToTimeoutId.count(luaId) == 1)
		{
			luaBindingIdToTimeoutId.erase(luaId);
		}
	}

	void Event::reportSdlErrors()
	{
		{
			std::string error = std::string(SDL_GetError());
			if (error.size())
			{
				Logger::instance()->logCritical("SDL::Error reported from SDL\n" + error);
				SDL_ClearError();
			}
		}

		{
			std::string error = std::string(IMG_GetError());
			if (error.size())
			{
				Logger::instance()->logCritical("SDL::Error reported from SDL_IMG\n" + error);
			}
		}

		{
			std::string error = std::string(TTF_GetError());
			if (error.size())
			{
				Logger::instance()->logCritical("SDL::Error reported from SDL_TTF\n" + error);
			}
		}

		{
			std::string error = std::string(Mix_GetError());
			if (error.size())
			{
				Logger::instance()->logCritical("SDL::Error reported from SDL_MIX\n" + error);
			}
		}
	}

	void Event::processEnterFrames(const double& frameTime)
	{
		processingOnEnterFrames = true;
		for (auto& priorityToCallbacks : onEnterFrameCallbacks)
		{
			auto& callbacks = priorityToCallbacks.second;
			for (auto it = callbacks.begin(); it != callbacks.end();)
			{
				const auto elePtr = (*it)->ptr.lock();

				if (elePtr != nullptr)
				{
					elePtr->enterFrame(frameTime);
					++it;
				}
				else
				{
					enterFrameObjects.erase((*it)->id);
					it = callbacks.erase(it);
				}
			}
		}

		for (auto it = timeoutCallbacks.begin(); it != timeoutCallbacks.end();)
		{
			auto& bundle = (*it);
			bundle->time -= frameTime;
			if (bundle->time <= 0)
			{
				if (bundle->luaId == -1)
				{
					const auto ptr = bundle->ptr.lock();
					if (ptr != nullptr)
					{
						ptr->onTimeoutCalled(bundle->id);
					}
				}
				else if (luaBindingIdToTimeoutId.count(bundle->luaId) == 1)
				{
					lua->callGlobalFunction(eventExecureTimeoutFunctionName, { std::to_string(bundle->luaId) });
					luaBindingIdToTimeoutId.erase(bundle->luaId);
				}

				it = timeoutCallbacks.erase(it);
			}
			else
			{
				++it;
			}
		}

		processingOnEnterFrames = false;

		for (const auto timeoutCallback : timeoutProcessedCallbacks)
		{
			timeoutCallbacks.push_back(timeoutCallback);
		}
		timeoutProcessedCallbacks.clear();

		for (const auto timeoutIdToRemove : timeoutProcessedRemoveCallbacks)
		{
			unregisterTimeoutCallback(timeoutIdToRemove);
		}
		timeoutProcessedRemoveCallbacks.clear();

		for (const auto cleanup : cleanupObjects)
		{
			unregisterOnEnterFrame(cleanup);
		}
		cleanupObjects.clear();
	}

	void Event::registerOnEnterFrame(std::shared_ptr<EnterFrameListener> listener, int priority)
	{
		if (enterFrameObjects.count(listener->getObjectIdEnterFrame()) == 1 || listener == nullptr)
		{
			return;
		}

		onEnterFrameCallbacks[priority].push_back(std::make_shared<EnterFrameListenerBundle>(listener));
		enterFrameObjects.insert(listener->getObjectIdEnterFrame());
	}

	void Event::unregisterOnEnterFrame(std::shared_ptr<EnterFrameListener> listener)
	{
		if (enterFrameObjects.count(listener->getObjectIdEnterFrame()) == 0 || listener == nullptr)
		{
			return;
		}

		if (processingOnEnterFrames)
		{
			cleanupObjects.push_back(listener);
			return;
		}

		for (auto& priorityToCallbacks : onEnterFrameCallbacks)
		{
			auto& callbacks = priorityToCallbacks.second;
			for (auto it = callbacks.begin(); it != callbacks.end();)
			{
				if ((*it)->id == listener->getObjectIdEnterFrame())
				{
					enterFrameObjects.erase((*it)->id);
					callbacks.erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
		}
	}

	int Event::registerTimeoutCallback(std::shared_ptr<EnterFrameListener> listener, double timeoutMS)
	{
		const auto bundle = std::make_shared<TimeoutBundle>(timeoutId++, listener, timeoutMS);

		if (processingOnEnterFrames)
		{
			timeoutProcessedCallbacks.push_back(bundle);
		}
		else
		{
			timeoutCallbacks.push_back(bundle);
		}

		return bundle->id;
	}

	void Event::unregisterTimeoutCallback(int id)
	{
		if (processingOnEnterFrames)
		{
			timeoutProcessedRemoveCallbacks.push_back(id);
		}
		else
		{
			for (auto it = timeoutCallbacks.begin(); it != timeoutCallbacks.end();)
			{
				if ((*it)->id == id)
				{
					it = timeoutCallbacks.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}

		}
	}

	void Event::onCleanup()
	{
		reportSdlErrors();
	}

	void Event::onEnterFrame(const double& frameTime)
	{
		processEvents();
		processEnterFrames(frameTime);
	}

	void Event::onBindLuaHooks(const std::shared_ptr<Lua>& lua)
	{
		lua->registerBoundFunction(eventSetTimeoutFunctionName, shared_from_this());
		lua->registerBoundFunction(eventClearTimeoutFunctionName, shared_from_this());

		lua->registerBoundFunction(eventRegisterOnEventFrameFunctionName, shared_from_this());
		lua->registerBoundFunction(eventUnregisterOnEnterFrameFunctionName, shared_from_this());
	}

	void Event::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == eventSetTimeoutFunctionName && obj->args.size() == 2)
		{
			const auto luaId = std::stoi(obj->args[0]);
			const auto callbackMs = std::stod(obj->args[1]);
			const auto timeoutId = setLuaTimeout(luaId, callbackMs);

			luaBindingIdToTimeoutId[luaId] = timeoutId;
		}
		else if (func == eventClearTimeoutFunctionName && obj->args.size() == 1) clearLuaTimeout(std::stoi(obj->args[0]));
		else if (func == eventRegisterOnEventFrameFunctionName && obj->args.size() == 1)
		{
			auto ptr = std::dynamic_pointer_cast<EnterFrameListener>(lua->getILuaObjectObjectForBindingId(obj->args[0]));
			registerOnEnterFrame(ptr);
			ptr->enterFrameActivated = true;
		}
		else if (func == eventUnregisterOnEnterFrameFunctionName&& obj->args.size() == 1)
		{
			auto ptr = std::dynamic_pointer_cast<EnterFrameListener>(lua->getILuaObjectObjectForBindingId(obj->args[0]));
			unregisterOnEnterFrame(ptr);
			ptr->enterFrameActivated = false;
		}
	}
}
