#pragma once

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
			EnterFrameListenerBundle(std::shared_ptr<EnterFrameListener> ptr) : id(ptr->getObjectIdEnterFrame()), ptr(ptr) {}
			const int id;
			const std::weak_ptr<EnterFrameListener> ptr;
		};

		class TimeoutBundle
		{
		public:
			TimeoutBundle(int id, int luaId, double time) : time(time), id(id), luaId(luaId) {}
			TimeoutBundle(int id, std::shared_ptr<EnterFrameListener> ptr, double time) : ptr(ptr), time(time), id(id), luaId(-1) {}

			const int id = 0, luaId = 0;
			const std::weak_ptr<EnterFrameListener> ptr;

			double time = 0.0;
		};

		std::shared_ptr<Input> input;
		std::shared_ptr<Thread> thread;
		std::shared_ptr<Lua> lua;

		int timeoutId = 100;
		bool processingOnEnterFrames = false;

		std::set<int> enterFrameObjects;
		std::map<int, std::list<std::shared_ptr<EnterFrameListenerBundle>>, std::greater<int>> onEnterFrameCallbacks;
		std::map<int, int> luaBindingIdToTimeoutId;
		std::list<std::shared_ptr<EnterFrameListener>> cleanupObjects;
		std::list<std::shared_ptr<TimeoutBundle>> timeoutCallbacks;
		std::list<std::shared_ptr<TimeoutBundle>> timeoutProcessedCallbacks;
		std::list<int> timeoutProcessedRemoveCallbacks;
		std::list<ApplicationEvent> events;

		void reportSdlErrors();
		void processEnterFrames(const double& frameTime);
		void processEvents();

		int setLuaTimeout(int luaId, double timeoutMS = 0.0);
		void clearLuaTimeout(int luaId);

	public:
		void bindInput(std::shared_ptr<Input> input);
		void bindThread(std::shared_ptr<Thread> thread);
		void bindLua(std::shared_ptr<Lua> lua);

		void pushEvent(const ApplicationEvent& event);
		const std::list<ApplicationEvent> getEvents();

		void registerOnEnterFrame(std::shared_ptr<EnterFrameListener> listener, int priority = 0.0);
		void unregisterOnEnterFrame(std::shared_ptr<EnterFrameListener> listener);

		int registerTimeoutCallback(std::shared_ptr<EnterFrameListener> listener, double timeoutMS);
		void unregisterTimeoutCallback(int id);

		void onBindLuaHooks(const std::shared_ptr<Lua>& lua) override;
		void onEnterFrame(const double& frameTime) override;
		void onCleanup() override;

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override { return "event"; };
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};
}
