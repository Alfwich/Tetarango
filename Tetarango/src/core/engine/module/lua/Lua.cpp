#include "Lua.h"
#include "engine/module/serialization/Hydrater.h"

namespace
{
	const auto dummyCode = std::string();
	const auto globalBindingId = std::string();
	const auto awCoreLibLocation = "res/lua/core/aw.lua";
	const auto contextIdBindingName = std::string("aw_cid");
	const auto boundFunctionsGlobalName = "aw_functions";
	const auto boundObjectsGlobalName = "aw_objects";

	const auto luaBindingObjectTypeNameFieldName = "aw_type";
	const auto primeInlineScript = std::string(boundObjectsGlobalName) + "={}\n" + std::string(boundFunctionsGlobalName) + "={}\n";

	const auto doStringMethodName = "doString";
	const auto doFileMethodName = "doFile";

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
		AW::Lua* svc = static_cast<AW::Lua*>(lua_touserdata(L, lua_upvalueindex(1)));
		int bundleId = (int)lua_tointeger(L, lua_upvalueindex(2));
		AW::LuaBoundObject* bundle = svc->getBoundObjectBundleForId(bundleId);

		if (bundle == nullptr)
		{
			return 0;
		}

		bundle->args.clear();

		const auto inTopSize = lua_gettop(L);
		for (unsigned int i = 1, end = inTopSize; i <= end; ++i)
		{
			const auto value = convertStackLocationToString(L, i);
			if (!value.empty())
			{
				bundle->args.push_back(value);
			}
		}

		bundle->returnValues.clear();
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

		return lua_gettop(L) - inTopSize;
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
			const auto bundle = asset->getAssetBundle(path, allowCached);
			if (bundle != nullptr)
			{
				fileScriptCache[path] = std::string(bundle->data.get(), bundle->size);
			}
		}

		if (fileScriptCache.count(path) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to load code at path=" + path + ", this does not exist");
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
			Logger::instance()->logCritical("Lua::Error running script\n" + err);
#if _DEBUG
			__debugbreak();
#endif
			lua_pop(L, 1);
		}
	}

	void Lua::createDefaultContext()
	{
		if (defaultContext == -1 && !isCleanedUp)
		{
			defaultContext = createNewContextAndSetActive();
			Logger::instance()->log("Lua::Created default context with contextId=" + std::to_string(defaultContext));
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

		primeContext(id);

		Logger::instance()->log("Lua::Created new execution context with id=" + std::to_string(id));

		return id;
	}

	int Lua::createNewContextAndSetActive(bool openLibs)
	{
		const auto contextId = createNewContext(openLibs);
		setActiveContext(contextId);

		return contextId;
	}

	bool Lua::setActiveContext(int id)
	{
		if (currentActiveContextId == id) return true;

		const auto prevContextId = currentActiveContextId;
		if (id == -1)
		{
			currentActiveContextId = defaultContext;
			return true;
		}

		if (contexts.count(id) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to change to context id=" + std::to_string(id) + ", as this context does not exist");
			currentActiveContextId = prevContextId;
			return false;
		}

		currentActiveContextId = id;
		return true;
	}

	void Lua::cleanupContext(int id)
	{
		if (contexts.count(id) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to cleanup context id=" + std::to_string(id) + ", no context exists for this id");
			return;
		}

		if (id == defaultContext)
		{
			Logger::instance()->logCritical("Lua::Failed to cleanup context id=" + std::to_string(id) + ", as this is the default context");
			return;
		}

		lua_close(contexts.at(id));
		contexts.erase(id);

		const auto strContextId = std::to_string(id);
		for (auto it = bindings.begin(); it != bindings.end();)
		{
			const auto key = (*it).first;
			if (std::get<0>(key) == strContextId)
			{
				// Cleanup bound objects for deleted context
				const auto bindingId = std::get<1>(key);
				if (!bindingId.empty() && keyToBindings.count(bindingId))
				{
					auto& bindingVector = keyToBindings[bindingId];
					for (auto it2 = bindingVector.begin(); it2 != bindingVector.end();)
					{
						if ((*it2) == key)
						{
							it2 = bindingVector.erase(it2);
						}
						else
						{
							++it2;
						}
					}
				}

				// Cleanup global functions for deleted context
				const auto functionName = std::get<2>(key);
				if (bindingId.empty() && !functionName.empty() && keyToBindings.count(functionName))
				{
					auto& bindingVector = keyToBindings[functionName];
					for (auto it2 = bindingVector.begin(); it2 != bindingVector.end();)
					{
						if ((*it2) == key)
						{
							it2 = bindingVector.erase(it2);
						}
						else
						{
							++it2;
						}
					}
				}


				it = bindings.erase(it);
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
		if (!script.empty())
		{
			Logger::instance()->log("Lua::Executing script path=" + path + ", for contextId=" + std::to_string(currentActiveContextId));
			executeLuaString(script);
		}
	}

	void Lua::executeLuaScriptForContext(std::string path, int contextId, bool allowCached)
	{
		const auto script = loadScriptForPath(path, allowCached);
		if (!script.empty())
		{
			executeLuaStringForContext(script, contextId);
		}
	}

	void Lua::executeLuaStringForContext(const std::string& script, int contextId)
	{
		if (contexts.count(contextId) == 0)
		{
			Logger::instance()->logCritical("Lua::Failed to execute script for context id=" + std::to_string(contextId) + ", this context does not exist\n" + script);
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
			Logger::instance()->logCritical("Lua::Failed to register function name=" + fnName + ", no active context available");
			return;
		}

		const auto bindingId = callbackObj == nullptr ? globalBindingId : callbackObj->getLuaBindingId();
		const auto functionBundleKey = std::make_tuple(std::to_string(currentActiveContextId), bindingId, fnName);

		bindingIdToBoundObject[bindingId] = callbackObj;
		bindings[functionBundleKey] = std::make_shared<LuaBoundObject>(fnName, fn, callbackObj, luaBindingAdapter);
		bindingsLuaAdapterMap[bindings.at(functionBundleKey)->bundleId] = bindings.at(functionBundleKey);
		keyToBindings[bindingId.empty() ? fnName : bindingId].push_back(functionBundleKey);

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

				const auto typeName = callbackObj->getAwType();
				lua_pushstring(L, typeName.c_str());
				lua_setfield(L, -2, luaBindingObjectTypeNameFieldName);
			}
		}

		lua_pushlightuserdata(L, (void*)this);
		lua_pushinteger(L, bindings.at(functionBundleKey)->bundleId);
		lua_pushcclosure(L, bindings.at(functionBundleKey)->luaFunction, 2);
		lua_setfield(L, -2, fnName.c_str());

		lua_pop(L, bindingId == globalBindingId ? 1 : 2);
	}

	void Lua::primeContext(int contextId)
	{
		executeLuaStringForContext(primeInlineScript + contextIdBindingName + "=" + std::to_string(contextId), contextId);
		registerBoundFunctionForContext(doStringMethodName, shared_from_this(), contextId);
		registerBoundFunctionForContext(doFileMethodName, shared_from_this(), contextId);
		executeLuaScriptForContext(awCoreLibLocation, contextId);
	}

	void Lua::callGlobalFunction(const std::string& function, const std::vector<std::string>& args)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to call global function name=" + function + ", no active context available");
			return;
		}

		lua_getglobal(L, boundFunctionsGlobalName);
		lua_getfield(L, -1, function.c_str());

		const auto isFn = lua_isfunction(L, -1);
		if (isFn)
		{
			for (const auto arg : args)
			{
				lua_pushstring(L, arg.c_str());
			}

			lua_call(L, (int)args.size(), 0);
		}

		lua_pop(L, isFn ? 1 : 2);
	}

	void Lua::callBoundFunction(const std::string& bindingId, const std::string& function, const std::vector<std::string>& args)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to call bound function for bindingId=" + bindingId + ", function name=" + function + ", no active context available");
			return;
		}

		lua_getglobal(L, boundObjectsGlobalName);
		lua_getfield(L, -1, bindingId.c_str());
		if (lua_istable(L, -1))
		{
			lua_getfield(L, -1, function.c_str());
			if (lua_isfunction(L, -1))
			{
				for (const auto arg : args)
				{
					lua_pushstring(L, arg.c_str());
				}

				lua_call(L, (int)args.size(), 0);
			}
			else
			{
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 2);
	}

	void Lua::callGlobalFunctionForContext(const std::string& function, int contextId, const std::vector<std::string>& args)
	{
		const auto prevContextId = currentActiveContextId;
		if (setActiveContext(contextId))
			callGlobalFunction(function, args);
		setActiveContext(prevContextId);
	}

	void Lua::callBoundFunctionForContext(const std::string& bindingId, const std::string& function, int contextId, const std::vector<std::string>& args)
	{
		const auto prevContextId = currentActiveContextId;
		if (setActiveContext(contextId))
			callBoundFunction(bindingId, function, args);
		setActiveContext(prevContextId);
	}

	void Lua::registerBoundFunction(const std::string& fnName, const std::shared_ptr<ILuaObject>& callbackObj)
	{
		registerFunction(fnName, nullptr, callbackObj);
	}

	void Lua::registerGlobalFunction(const std::string& fnName, void(*fn)(LuaBoundObject*))
	{
		registerFunction(fnName, fn, nullptr);
	}

	void Lua::registerBoundFunctionForContext(const std::string& fnName, const std::shared_ptr<ILuaObject>& callbackObj, int contextId)
	{
		const auto prevContextId = currentActiveContextId;
		if (setActiveContext(contextId))
			registerBoundFunction(fnName, callbackObj);
		setActiveContext(prevContextId);
	}

	void Lua::registerGlobalFunctionForContext(const std::string& fnName, void(*fn)(LuaBoundObject*), int contextId)
	{
		const auto prevContextId = currentActiveContextId;
		if (setActiveContext(contextId))
			registerGlobalFunction(fnName, fn);
		setActiveContext(prevContextId);
	}

	void Lua::unregisterBoundFunction(const std::string& bindingId, const std::string& fnName)
	{
		if (keyToBindings.count(bindingId) != 0)
		{
			auto& fns = keyToBindings.at(bindingId);
			for (auto it = fns.begin(), end = fns.end(); it != end;)
			{
				const auto bindingKey = *it;
				if (std::get<2>(bindingKey) == fnName)
				{
					if (bindings.count(bindingKey) == 1)
					{
						bindings.erase(bindingKey);
					}

					it = fns.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}

			if (fns.empty())
			{
				keyToBindings.erase(bindingId);
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

			lua_pop(L, isTable ? 1 : 2);
		}
	}

	void Lua::unregisterBoundFunctions(const std::string& bindingId)
	{
		if (keyToBindings.count(bindingId) != 0)
		{
			auto& fns = keyToBindings.at(bindingId);
			for (auto it = fns.begin(); it != fns.end(); ++it)
			{
				const auto bindingKey = (*it);
				if (bindings.count(bindingKey) == 1)
				{
					const auto bundle = bindings.at(bindingKey);
					if (bindingsLuaAdapterMap.count(bundle->bundleId) == 1)
					{
						bindingsLuaAdapterMap.erase(bundle->bundleId);
					}

					bindings.erase(bindingKey);
				}
			}

			keyToBindings.erase(bindingId);
		}

		if (bindingIdToBoundObject.count(bindingId) == 1)
		{
			bindingIdToBoundObject.erase(bindingId);
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
		if (keyToBindings.count(fnName) != 0)
		{
			auto& fns = keyToBindings.at(fnName);
			for (auto it = fns.begin(); it != fns.end(); ++it)
			{
				const auto bindingKey = (*it);
				if (bindings.count(bindingKey) == 1)
				{
					const auto bundle = bindings.at(bindingKey);
					if (bindingsLuaAdapterMap.count(bundle->bundleId) == 1)
					{
						bindingsLuaAdapterMap.erase(bundle->bundleId);
					}

					bindings.erase(bindingKey);
				}
			}

			keyToBindings.erase(fnName);
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

	void Lua::registerObjectImplementation(const std::string& implFilePath, const std::string& implKey)
	{
		if (defaultContext != -1)
		{
			callGlobalFunctionForContext("AW_registerObjectImpl", defaultContext, { implFilePath, implKey });

			if (registeredImpls.count(implKey) == 1)
			{
				Logger::instance()->logCritical("Lua::Reregistered object implementation for implKey=" + implKey + ", with implFilePath=" + implFilePath);

			}
			else
			{
				registeredImpls.emplace(implKey, implFilePath);
				Logger::instance()->log("Lua::Registered object implementation for implKey=" + implKey);
			}

		}
		else
		{
			Logger::instance()->logCritical("Lua::Failed to register object implementation for implKey=" + implKey + ", with implFilePath=" + implFilePath + ", defaultContext could not be acquired");
		}
	}

	void Lua::setObjectImplementation(const std::string& bindingId, const std::string& implKey)
	{
		callGlobalFunctionForContext("AW_setObjectImpl", defaultContext, { bindingId, implKey });
	}

	bool Lua::hasObjectImplementation(const std::string& implKey)
	{
		return registeredImpls.count(implKey) == 1;
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
			Logger::instance()->logCritical("Lua::Failed to get global double, context could not be acquired");
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
			Logger::instance()->logCritical("Lua::Failed to get global string, context could not be acquired");
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

	LuaBoundObject* Lua::getBoundObjectBundleForId(int id)
	{
		return (bindingsLuaAdapterMap.count(id) == 1)
			? bindingsLuaAdapterMap.at(id).get()
			: nullptr;
	}

	void Lua::onInit()
	{
		createDefaultContext();

		for (const auto implKeyToImplFilePath : registeredImpls)
		{
			callGlobalFunctionForContext("AW_registerObjectImpl", defaultContext, { implKeyToImplFilePath.second, implKeyToImplFilePath.first });
			Logger::instance()->log("Lua::Registered object implementation for implKey=" + implKeyToImplFilePath.first);
		}
	}

	void Lua::onLoadResources()
	{
		callGlobalFunctionForContext("AW_loadImplResources", defaultContext);
	}

	void Lua::onCleanup()
	{
		cleanupUserCreatedContexts();
		cleanupDefaultContext();

		contexts.clear();
		fileScriptCache.clear();
		bindings.clear();
		keyToBindings.clear();
		bindingsLuaAdapterMap.clear();
	}

	void Lua::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == doStringMethodName && obj->args.size() == 1) executeLuaString(obj->args[0]);
		else if (func == doFileMethodName && obj->args.size() == 1) executeLuaScript(obj->args[0]);
		else if (func == doFileMethodName && obj->args.size() == 2) executeLuaScript(obj->args[0], obj->args[1] == "1");
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

	void Lua::fireTimeoutCallback(int timeoutBindingId)
	{
		// TODO: Move into Event module
		callGlobalFunctionForContext("AW_exe_timeout", defaultContext, { std::to_string(timeoutBindingId) });
	}

	std::shared_ptr<ILuaObject> Lua::getILuaObjectObjectForBindingId(std::string luaBindingId)
	{
		return (bindingIdToBoundObject.count(luaBindingId) == 1)
			? bindingIdToBoundObject.at(luaBindingId)
			: nullptr;
	}
}
