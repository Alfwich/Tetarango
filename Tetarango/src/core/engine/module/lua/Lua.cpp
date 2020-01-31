#include "Lua.h"

namespace
{
	std::string dummyCode = "";
}

namespace AW
{
	void Lua::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	std::string& Lua::loadScriptForPath(const std::string& path, bool allowCached)
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

		if (luaL_dostring(L, script.c_str()) == LUA_OK)
		{
			lua_getglobal(L, "a");
			if (lua_isnumber(L, -1))
			{
				std::cout << (float)lua_tonumber(L, -1) << std::endl;
			}
		}
		else
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

	int Lua::createNewContext()
	{
		const auto id = nextInactiveContextId++;
		const auto L = luaL_newstate();
		contexts[id] = L;
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
			Logger::instance()->logCritical("Lua::Failed to change to context id:" + std::to_string(id) + ", as this context does not exist!");
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

	void Lua::executeLuaScript(std::string path, bool allowCached)
	{
		const auto script = loadScriptForPath(path, allowCached);
		executeLuaString(script);
	}

	void Lua::executeLuaString(const std::string& script)
	{
		executeLua(script);
	}

	void Lua::onInit()
	{
		createDefaultContext();
		executeLuaScript("res/lua/test.lua");
	}

	void Lua::onCleanup()
	{
		cleanupUserCreatedContexts();
		cleanupDefaultContext();
		contexts.clear();
	}
}
