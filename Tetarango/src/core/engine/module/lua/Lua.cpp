#include "Lua.h"

namespace
{
	const std::string dummyCode = "";
	const auto awCoreLibLocation = "res/lua/aw-core.lua";
	const auto boundFunctionsGlobalName = "global_functions";
	const auto boundObjectsGlobalName = "bound_objects";
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
			setActiveContextId(defaultContext);
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

	void Lua::setActiveContextId(int id)
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

		if (currentActiveContextId == id)
		{
			setActiveContextId(defaultContext);
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
		setActiveContextId(contextId);
		executeLuaString(script);
		setActiveContextId(prevContextId);
	}

	void Lua::registerFunction(const std::string& fnName, int(*fn)(void), const std::shared_ptr<ILuaCallbackTarget>& callbackObj)
	{
		const auto L = getCurrentContextLuaState();
		if (L == nullptr)
		{
			Logger::instance()->logCritical("Lua::Failed to register function name: " + fnName + ", no active context available");
			return;
		}

		const auto bindingId = std::to_string(callbackObj == nullptr ? 0 : callbackObj->getLuaBindingId());
		const auto functionBundleKey = std::to_string(currentActiveContextId) + "-" + bindingId + "-" + fnName;
		functionBundles[functionBundleKey] =
			LuaFunctionBundle(fnName, fn, callbackObj,
				[](lua_State* L)
				{
					LuaFunctionBundle* bundle = static_cast<LuaFunctionBundle*>(lua_touserdata(L, lua_upvalueindex(1)));

					if (bundle->callback != nullptr)
					{
						return bundle->callback();
					}

					const auto callbackObjPtr = bundle->callbackObj.lock();
					if (callbackObjPtr != nullptr)
					{
						return callbackObjPtr->onLuaCallback(bundle->fnName);
					}

					return 0;
				}
		);

		if (bindingId == "0")
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

		lua_pushlightuserdata(L, &functionBundles.at(functionBundleKey));
		lua_pushcclosure(L, functionBundles.at(functionBundleKey).luaFunction, 1);
		lua_setfield(L, -2, fnName.c_str());
		lua_pop(L, 1);
	}

	void Lua::registerBoundFunction(const std::string& fnName, const std::shared_ptr<ILuaCallbackTarget>& callbackObj)
	{
		registerFunction(fnName, nullptr, callbackObj);
	}

	void Lua::registerGlobalFunction(const std::string& fnName, int(*fn)(void))
	{
		registerFunction(fnName, fn, nullptr);
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
		return lua_isnumber(L, -1) ? (int)lua_tonumber(L, -1) : 0;
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
		return lua_isnumber(L, -1) ? (double)lua_tonumber(L, -1) : 0.0;
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
		return lua_isstring(L, -1) ? std::string(lua_tostring(L, -1)) : std::string();
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
				const auto valueType = lua_type(L, -1);
				switch (valueType)
				{
				case LUA_TNUMBER:
					result.push_back(std::to_string(lua_tonumber(L, -1)));
					break;

				case LUA_TSTRING:
					result.push_back(std::string(lua_tostring(L, -1)));
					break;

				case LUA_TBOOLEAN:
					result.push_back(std::to_string(lua_toboolean(L, -1)));
					break;
				}
				lua_pop(L, 1);
			}
		}

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

				std::string key, value;
				switch (keyType)
				{
				case LUA_TNUMBER:
					key = std::to_string(lua_tonumber(L, -2));
					break;

				case LUA_TSTRING:
					key = std::string(lua_tostring(L, -2));
					break;

				case LUA_TBOOLEAN:
					key = std::to_string(lua_toboolean(L, -2));
					break;
				}

				switch (valueType)
				{
				case LUA_TNUMBER:
					value = std::to_string(lua_tonumber(L, -1));
					break;

				case LUA_TSTRING:
					value = std::string(lua_tostring(L, -1));
					break;

				case LUA_TBOOLEAN:
					value = std::to_string(lua_toboolean(L, -1));
					break;
				}

				if (!key.empty() && !value.empty())
				{
					result[key] = value;
				}

				lua_pop(L, 1);
			}
		}

		return result;
	}

	void Lua::onInit()
	{
		class TestBindingObject : public ILuaCallbackTarget
		{
		public:
			int getLuaBindingId() override
			{
				return 11;
			};

			int onLuaCallback(const std::string& func) override
			{
				std::cout << func << " called" << std::endl;
				return 0;
			}
		};

		createDefaultContext();
		registerGlobalFunction("testFn", []() { std::cout << "doSomething" << std::endl; return 0; });
		registerGlobalFunction("testFn2", []() { std::cout << "doSomething too!" << std::endl; return 0; });
		executeLuaString("global_functions.testFn()");
		executeLuaString("global_functions.testFn2()");

		const auto testObj = std::make_shared<TestBindingObject>();
		registerBoundFunction("testFn", testObj);
		registerBoundFunction("testFn2", testObj);
		executeLuaString("bound_objects[\"" + std::to_string(testObj->getLuaBindingId()) + "\"].testFn()");
		executeLuaString("bound_objects[\"" + std::to_string(testObj->getLuaBindingId()) + "\"].testFn2()");

		executeLuaScript("res/lua/test.lua");
		const auto testNum = getGlobalInt("testNum");
		const auto testDouble = getGlobalDouble("testDouble");
		const auto testString = getGlobalString("testString");
		const auto testTable = getGlobalTable("testTable");
		const auto testRecord = getGlobalRecord("testRecord");
		int x = 5;
	}

	void Lua::onCleanup()
	{
		cleanupUserCreatedContexts();
		cleanupDefaultContext();
		contexts.clear();
		fileScriptCache.clear();
		functionBundles.clear();
	}
}
