#include "Lua.h"

#include "lua.hpp"

namespace
{
	lua_State* L;
}

namespace AW
{
	void Lua::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	std::string& Lua::loadScriptForPath(const std::string& path)
	{
		if (scripts.count(path) == 0)
		{
			const auto bundle = asset->getAssetBundle(path);
			if (bundle != nullptr)
			{
				scripts[path] = std::string(bundle->data.get(), bundle->size);
			}
		}

		return scripts.at(path);
	}

	void Lua::onInit()
	{
		L = luaL_newstate();
		executeLuaScript("res/lua/test.lua");
	}

	void Lua::onCleanup()
	{
		lua_close(L);
		L = nullptr;
	}

	void Lua::executeLuaScript(std::string path)
	{
		if (isCleanedUp) return;

		const auto script = loadScriptForPath(path);

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
}
