#pragma once

#include "thread/INotifyOnCompletion.h"
#include "lua/ILuaObject.h"
#include "GameConfig.h"

namespace AW
{
	class Lua;

	class IBaseModule : public INotifyOnCompletion, public ILuaObject, public std::enable_shared_from_this<IBaseModule>
	{
	public:
		std::shared_ptr<Config::GameConfig> gameConfig;
		bool isCleanedUp = false;

		virtual void onInit() { /* NO-OP */ };
		virtual void onLoadResources() { /* NO-OP */ };
		virtual void onReady() { /* NO-OP */ };
		virtual void onBindLuaHooks(const std::shared_ptr<Lua>& lua) { /* NO-OP */ };
		virtual void onEnterFrame(const double& frameTime) { onEnterFrame(); };
		virtual void onEnterFrame() { /* NO-OP */ };
		virtual void onCleanup() { /* NO-OP */ };
		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result) { /* NO-OP */ };
		virtual void onWorkError(WORKER_ID workerId, WorkerTaskCode type) { /* NO-OP */ };

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override { return "module"; };
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override {};
	};
}

#include "engine/module/lua/Lua.h"
