#include "Lua.h"

namespace
{
	const auto dummyCode = std::string();
	const auto globalBindingId = std::string();
	const auto awCoreLibLocation = "res/lua/aw-core.lua";
	const auto boundFunctionsGlobalName = "aw_functions";
	const auto boundObjectsGlobalName = "aw_objects";

	std::string convertStackLocationToString(lua_State* L, unsigned int loc)
	{
		const auto valueType = lua_type(L, loc);

		switch (valueType)
		{
		case LUA_TNUMBER:
			return std::to_string(lua_tonumber(L, loc));

		case LUA_TSTRING:
			return std::string(lua_tostring(L, loc));

		case LUA_TBOOLEAN:
			return std::to_string(lua_toboolean(L, -1));
		}

		return std::string();
	}

	int luaBindingAdapter(lua_State* L)
	{
		std::vector<std::string> returnArgs;
		AW::LuaBoundObject* bundle = static_cast<AW::LuaBoundObject*>(lua_touserdata(L, lua_upvalueindex(1)));

		bundle->args = lua_gettop(L);
		for (unsigned int i = 1; i <= bundle->args; ++i)
		{
			if (bundle->argV.size() == i - 1)
			{
				bundle->argV.push_back(convertStackLocationToString(L, i));
			}
			else
			{
				bundle->argV[i - 1] = convertStackLocationToString(L, i);
			}
		}

		if (bundle->callback != nullptr)
		{
			bundle->callback(bundle);
		}
		else
		{
			const auto callbackObjPtr = bundle->callbackObj.lock();
			if (callbackObjPtr != nullptr)
			{
				callbackObjPtr->onLuaCallback(bundle->fnName, bundle);
			}

		}

		for (const auto returnValue : bundle->returnValues)
		{
			lua_pushstring(L, returnValue.c_str());
		}

		return (int)bundle->returnValues.size();
	}
}

namespace AW
{
	void Lua::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	const std::string& Lua::loadScriptForPath(const std::string& path, bool allowCached)
	{
		if (!allowCached || fileScriptCache.count(path) == 0)
		{
			const auto bundle = asset->getAssetBundle(path);
			if (bundle != nullptr)
			{
				fileScriptCache[path] = std::string(bundle->data.get(), bundle->size);
			}
		}

		if (fileScriptCache.count(path) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to load code at path:" + path + ", this does not exist");
			return dummyCode;
		}

		return fileScriptCache.at(path);
	}

	void Lua::executeLua(const std::string& script)
	{
		if (isCleanedUp)
		{
			Logger::instance()->logCritical("Lua::Failed to run script, Lua module has been cleaned up\n" + script);
			return;
		}

		if (currentActiveContextId == -1)
		{
			Logger::instance()->logCritical("Lua::Failed to run script, no active context available\n" + script);
			return;
		}

		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to run script, context could not be acquired\n" + script);
			return;
		}

		if (luaL_dostring(L, script.c_str()) != LUA_OK)
		{
			std::string err = lua_tostring(L, -1);
			Logger::instance()->logCritical("Lua::Error running script: " + err);
		}
	}

	void Lua::createDefaultContext()
	{
		if (defaultContext == -1 && !isCleanedUp)
		{
			defaultContext = createNewContext();
			setActiveContext(defaultContext);
		}
	}

	void Lua::cleanupDefaultContext()
	{
		if (defaultContext != -1)
		{
			lua_close(contexts.at(defaultContext));
			defaultContext = -1;
		}
	}

	void Lua::cleanupUserCreatedContexts()
	{
		std::vector<int> ids;
		for (const auto idsToContexts : contexts) ids.emplace_back(idsToContexts.first);
		for (const auto id : ids)
		{
			if (id != defaultContext)
				cleanupContext(id);
		}
	}

	lua_State* Lua::getCurrentContextLuaState()
	{
		return (contexts.count(currentActiveContextId) != 0) ? contexts.at(currentActiveContextId) : nullptr;
	}

	int Lua::createNewContext(bool openLibs)
	{
		const auto id = nextInactiveContextId++;
		const auto L = luaL_newstate();

		if (openLibs)
		{
			luaL_openlibs(L);
		}

		contexts[id] = L;

		executeLuaScriptForContext(awCoreLibLocation, id);

		return id;
	}

	int Lua::createNewContextAndSetActive(bool openLibs)
	{
		const auto contextId = createNewContext(openLibs);
		setActiveContext(contextId);

		return contextId;
	}

	void Lua::setActiveContext(int id)
	{
		if (id == -1)
		{
			currentActiveContextId = defaultContext;
			return;
		}

		if (contexts.count(id) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to change to context id:" + std::to_string(id) + ", as this context does not exist");
			currentActiveContextId = defaultContext;
			return;
		}

		currentActiveContextId = id;
	}

	void Lua::cleanupContext(int id)
	{
		if (contexts.count(id) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to cleanup context id:" + std::to_string(id) + ", no context exists for this id");
			return;
		}

		if (id == defaultContext)
		{
			Logger::instance()->logCritical("Lua::Failed to cleanup context id:" + std::to_string(id) + ", as this is the default context");
			return;
		}

		lua_close(contexts.at(id));
		contexts.erase(id);

		const auto strContextId = std::to_string(id);
		for (auto it = boundObjects.begin(); it != boundObjects.end();)
		{
			if (std::get<0>((*it).first) == strContextId)
			{
				it = boundObjects.erase(it);
			}
			else
			{
				++it;
			}
		}

		if (currentActiveContextId == id)
		{
			setActiveContext(defaultContext);
		}
	}

	void Lua::executeLuaString(const std::string& script)
	{
		executeLua(script);
	}

	void Lua::executeLuaScript(std::string path, bool allowCached)
	{
		const auto script = loadScriptForPath(path, allowCached);
		executeLuaString(script);
	}

	void Lua::executeLuaScriptForContext(std::string path, int contextId, bool allowCached)
	{
		const auto script = loadScriptForPath(path, allowCached);
		executeLuaStringForContext(script, contextId);
	}

	void Lua::executeLuaStringForContext(const std::string& script, int contextId)
	{
		if (contexts.count(contextId) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to execute script for context id: " + std::to_string(contextId) + ", this context does not exist\n" + script);
			return;
		}

		const auto prevContextId = currentActiveContextId;
		setActiveContext(contextId);
		executeLuaString(script);
		setActiveContext(prevContextId);
	}

	void Lua::registerFunction(const std::string& fnName, void(*fn)(LuaBoundObject*), const std::shared_ptr<ILuaObject>& callbackObj)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to register function name: " + fnName + ", no active context available");
			return;
		}

		const auto bindingId = callbackObj == nullptr ? globalBindingId : callbackObj->getLuaBindingId();
		const auto functionBundleKey = std::make_tuple(std::to_string(currentActiveContextId), bindingId, fnName);
		boundObjects.emplace(std::piecewise_construct, functionBundleKey, std::make_tuple(fnName, fn, callbackObj, luaBindingAdapter));

		if (bindingId == globalBindingId)
		{
			lua_getglobal(L, boundFunctionsGlobalName);
		}
		else
		{
			lua_getglobal(L, boundObjectsGlobalName);
			lua_getfield(L, -1, bindingId.c_str());

			if (lua_isnil(L, -1))
			{
				lua_pop(L, 1);
				lua_newtable(L);
				lua_setfield(L, -2, bindingId.c_str());
				lua_getfield(L, -1, bindingId.c_str());
			}
		}

		lua_pushlightuserdata(L, &boundObjects.at(functionBundleKey));
		lua_pushcclosure(L, boundObjects.at(functionBundleKey).luaFunction, 1);
		lua_setfield(L, -2, fnName.c_str());

		lua_pop(L, bindingId == globalBindingId ? 1 : 2);
	}


	void Lua::registerBoundFunction(const std::string& fnName, const std::shared_ptr<ILuaObject>& callbackObj)
	{
		registerFunction(fnName, nullptr, callbackObj);
	}

	void Lua::registerGlobalFunction(const std::string& fnName, void(*fn)(LuaBoundObject*))
	{
		registerFunction(fnName, fn, nullptr);
	}

	void Lua::unregisterBoundFunctions(const std::shared_ptr<ILuaObject>& obj)
	{
		const auto bindingId = obj->getLuaBindingId();
		for (auto it = boundObjects.begin(); it != boundObjects.end();)
		{
			if (std::get<1>((*it).first) == bindingId)
			{
				it = boundObjects.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (const auto idtoContext : contexts)
		{
			const auto L = idtoContext.second;
			lua_getglobal(L, boundObjectsGlobalName);
			lua_getfield(L, -1, bindingId.c_str());

			const auto isTable = lua_istable(L, -1);
			if (isTable)
			{
				lua_pop(L, 1);
				lua_pushnil(L);
				lua_setfield(L, -2, bindingId.c_str());
			}

			lua_pop(L, isTable ? 1 : 2);
		}
	}

	void Lua::unregisterGlobalFunctions(const std::string& fnName)
	{
		for (auto it = boundObjects.begin(); it != boundObjects.end();)
		{
			if (std::get<2>((*it).first) == fnName)
			{
				it = boundObjects.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (const auto idtoContext : contexts)
		{
			const auto L = idtoContext.second;
			lua_getglobal(L, boundFunctionsGlobalName);
			lua_getfield(L, -1, fnName.c_str());

			const auto isFn = lua_isfunction(L, -1);
			if (isFn)
			{
				lua_pop(L, 1);
				lua_pushnil(L);
				lua_setfield(L, -2, fnName.c_str());
			}

			lua_pop(L, isFn ? 1 : 2);
		}
	}

	int Lua::getGlobalInt(const std::string& name)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to get global int, context could not be acquired");
			return 0;
		}

		lua_getglobal(L, name.c_str());
		const auto result = lua_isnumber(L, -1) ? (int)lua_tonumber(L, -1) : 0;
		lua_pop(L, 1);

		return result;
	}

	double Lua::getGlobalDouble(const std::string& name)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to get global int, context could not be acquired");
			return 0.0;
		}

		lua_getglobal(L, name.c_str());
		const auto result = lua_isnumber(L, -1) ? (double)lua_tonumber(L, -1) : 0.0;
		lua_pop(L, 1);

		return result;
	}

	std::string Lua::getGlobalString(const std::string& name)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to get global int, context could not be acquired");
			return std::string();
		}

		lua_getglobal(L, name.c_str());
		const auto result = lua_isstring(L, -1) ? std::string(lua_tostring(L, -1)) : std::string();
		lua_pop(L, 1);

		return result;
	}

	std::vector<std::string> Lua::getGlobalTable(const std::string& name)
	{
		auto result = std::vector<std::string>();
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to get global table, context could not be acquired");
			return result;
		}

		lua_getglobal(L, name.c_str());
		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2) != 0)
			{
				result.push_back(convertStackLocationToString(L, -1));
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 1);

		return result;
	}

	std::unordered_map<std::string, std::string> Lua::getGlobalRecord(const std::string& name)
	{
		auto result = std::unordered_map<std::string, std::string>();

		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to get global record, context could not be acquired");
			return result;
		}

		lua_getglobal(L, name.c_str());

		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2) != 0)
			{
				const auto keyType = lua_type(L, -2);
				const auto valueType = lua_type(L, -1);

				auto key = convertStackLocationToString(L, -2);
				auto value = convertStackLocationToString(L, -1);

				if (!key.empty() && !value.empty())
				{
					result[key] = value;
				}

				lua_pop(L, 1);
			}
		}

		lua_pop(L, 1);

		return result;
	}

	void Lua::onInit()
	{
		createDefaultContext();
	}

	void Lua::onCleanup()
	{
		cleanupUserCreatedContexts();
		cleanupDefaultContext();

		contexts.clear();
		fileScriptCache.clear();
		boundObjects.clear();
	}

	std::string Lua::getLuaBindingId()
	{
		return "lua";
	}

	void Lua::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == "runScript" && !obj->argV.empty()) executeLuaString(obj->argV[0]);
	}

	std::unordered_map<int, int> Lua::debugInfo()
	{
		auto result = std::unordered_map<int, int>();

		for (const auto idtoContext : contexts)
		{
			result[idtoContext.first] = lua_gettop(idtoContext.second);
		}

		return result;
	}
}
