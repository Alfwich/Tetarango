#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/asset/Asset.h"
#include "engine/module/asset/ResourceBundle.h"

namespace AW
{
	class Lua : public IBaseModule
	{
		std::shared_ptr<Asset> asset;

		std::unordered_map<std::string, std::string> scripts;

		std::string& loadScriptForPath(const std::string& path);

	public:
		void bindAsset(std::shared_ptr<Asset> asset);

		void onInit() override;
		void onCleanup() override;

		void executeLuaScript(std::string path);
	};
}
