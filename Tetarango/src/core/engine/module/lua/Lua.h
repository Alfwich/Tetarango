#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"
#include "engine/module/asset/ResourceBundle.h"
#include "LuaBoundObject.h"
#include "ILuaObject.h"

#include "lua.hpp"

namespace
{
	std::hash<std::string> hasher;
	class tuple_hash : public std::unary_function<std::tuple<std::string, std::string, std::string>, std::size_t>
	{
	public:
		std::size_t operator()(const std::tuple<std::string, std::string, std::string>& k) const
		{
			return hasher(std::get<0>(k) + std::get<1>(k) + std::get<2>(k));
		}
	};
}

namespace AW
{
	class Lua : public IBaseModule
	{
		std::shared_ptr<Asset> asset;
		int nextInactiveContextId = 0, currentActiveContextId = -1, defaultContext = -1;

		std::unordered_map<std::tuple<std::string, std::string, std::string>, LuaBoundObject, tuple_hash> bindings;
		std::unordered_map<std::string, std::vector<std::tuple<std::string, std::string, std::string>>> keyToBindings;

		std::unordered_map<std::string, std::string> fileScriptCache;
		std::unordered_map<std::string, std::string> registeredImpls;
		std::unordered_map<int, lua_State*> contexts;

		const std::string& loadScriptForPath(const std::string& path, bool allowCached = true);

		void executeLua(const std::string& script);
		void createDefaultContext();

		void cleanupDefaultContext();
		void cleanupUserCreatedContexts();

		lua_State* getCurrentContextLuaState();

		void registerFunction(const std::string& fnName, void(*fn)(LuaBoundObject*), const std::shared_ptr<ILuaObject>& callbackObj);

		void primeContext(int contextId);

	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		int createNewContext(bool openLibs = true);
		int createNewContextAndSetActive(bool openLibs = true);
		bool setActiveContext(int id);
		void cleanupContext(int id);

		void executeLuaScript(std::string path, bool allowCached = true);
		void executeLuaString(const std::string& script);

		void executeLuaScriptForContext(std::string path, int contextId, bool allowCached = true);
		void executeLuaStringForContext(const std::string& script, int contextId);

		void callGlobalFunction(const std::string& function, const std::vector<std::string>& args = std::vector<std::string>());
		void callBoundFunction(const std::string& bindingId, const std::string& function, const std::vector<std::string>& args = std::vector<std::string>());

		void callGlobalFunctionForContext(const std::string& function, int contextId, const std::vector<std::string>& args = std::vector<std::string>());
		void callBoundFunctionForContext(const std::string& bindingId, const std::string& function, int contextId, const std::vector<std::string>& args = std::vector<std::string>());

		void registerBoundFunction(const std::string& fnName, const std::shared_ptr<ILuaObject>& callbackObj);
		void registerGlobalFunction(const std::string& fnName, void(*fn)(LuaBoundObject*));

		void registerBoundFunctionForContext(const std::string& fnName, const std::shared_ptr<ILuaObject>& callbackObj, int contextId);
		void registerGlobalFunctionForContext(const std::string& fnName, void(*fn)(LuaBoundObject*), int contextId);

		void unregisterBoundFunction(const std::string& bindingId, const std::string& fnName = std::string());
		void unregisterBoundFunctions(const std::string& bindingId);

		void unregisterGlobalFunctions(const std::string& fnName);

		void registerObjectImplementation(const std::string& implFilePath, const std::string& implKey);
		void setObjectImplementation(const std::string& bindingId, const std::string& implKey);

		int getGlobalInt(const std::string& name);
		double getGlobalDouble(const std::string& name);
		std::string getGlobalString(const std::string& name);
		std::vector<std::string> getGlobalTable(const std::string& name);
		std::unordered_map<std::string, std::string> getGlobalRecord(const std::string& name);

		void onInit() override;
		void onEnterFrame(const double& frameTime);
		void onCleanup() override;

		// Inherited via IBaseModule
		virtual std::string getLuaBindingId() override;
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;

		std::unordered_map<int, int> debugInfo();
	};
}
