#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"
#include "engine/module/asset/ResourceBundle.h"
#include "ILuaCallbackTarget.h"

#include "lua.hpp"

namespace AW
{
	class LuaFunctionBundle
	{
	public:
		LuaFunctionBundle() {};
		LuaFunctionBundle(const std::string& fnName, int (callback)(void), std::shared_ptr<ILuaCallbackTarget> callbackObj, lua_CFunction lFn)
			: fnName(fnName), callback(callback), callbackObj(callbackObj), luaFunction(lFn)
		{
		}

		std::string fnName;
		lua_CFunction luaFunction;
		int (*callback)(void);
		std::weak_ptr<ILuaCallbackTarget> callbackObj;
	};

	class Lua : public IBaseModule
	{
		std::shared_ptr<Asset> asset;
		int nextInactiveContextId = 0, currentActiveContextId = -1, defaultContext = -1;

		std::unordered_map<std::string, LuaFunctionBundle> functionBundles;
		std::unordered_map<std::string, std::string> fileScriptCache;
		std::unordered_map<int, lua_State*> contexts;

		const std::string& loadScriptForPath(const std::string& path, bool allowCached = true);

		void executeLua(const std::string& script);
		void createDefaultContext();

		void cleanupDefaultContext();
		void cleanupUserCreatedContexts();

		lua_State* getCurrentContextLuaState();

		void registerFunction(const std::string& fnName, int(*fn)(void), const std::shared_ptr<ILuaCallbackTarget>& callbackObj);

	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		int createNewContext(bool openLibs = true);
		void setActiveContextId(int id);
		void cleanupContext(int id);

		void executeLuaScript(std::string path, bool allowCached = true);
		void executeLuaString(const std::string& script);

		void executeLuaScriptForContext(std::string path, int contextId, bool allowCached = true);
		void executeLuaStringForContext(const std::string& script, int contextId);

		void registerBoundFunction(const std::string& fnName, const std::shared_ptr<ILuaCallbackTarget>& callbackObj);
		void registerGlobalFunction(const std::string& fnName, int(*fn)(void));

		int getGlobalInt(const std::string& name);
		double getGlobalDouble(const std::string& name);
		std::string getGlobalString(const std::string& name);
		std::vector<std::string> getGlobalTable(const std::string& name);
		std::unordered_map<std::string, std::string> getGlobalRecord(const std::string& name);

		void onInit() override;
		void onCleanup() override;
	};
}
