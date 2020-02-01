#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"
#include "engine/module/asset/ResourceBundle.h"

#include "lua.hpp"

namespace AW
{
	class Lua : public IBaseModule
	{
		std::shared_ptr<Asset> asset;
		int nextInactiveContextId = 0, currentActiveContextId = -1, defaultContext = -1;

		std::unordered_map<std::string, std::string> fileScriptCache;
		std::unordered_map<int, lua_State*> contexts;

		const std::string& loadScriptForPath(const std::string& path, bool allowCached = true);

		void executeLua(const std::string& script);
		void createDefaultContext();

		void cleanupDefaultContext();
		void cleanupUserCreatedContexts();

		lua_State* getCurrentContextLuaState();

	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		void onInit() override;
		void onCleanup() override;

		int createNewContext(bool openLibs = true);
		void setActiveContextId(int id);
		void cleanupContext(int id);

		void executeLuaScript(std::string path, bool allowCached = true);
		void executeLuaString(const std::string& script);

		void executeLuaScriptForContext(std::string path, int contextId, bool allowCached = true);
		void executeLuaStringForContext(const std::string& script, int contextId);

		int getGlobalInt(const std::string& name);
		double getGlobalDouble(const std::string& name);
		std::string getGlobalString(const std::string& name);
		std::vector<std::string> getGlobalTable(const std::string& name);
		std::unordered_map<std::string, std::string> getGlobalRecord(const std::string& name);
	};
}
