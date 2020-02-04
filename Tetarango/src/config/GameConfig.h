#pragma once

#include "BaseConfig.h"

namespace Config
{
	enum class Param
	{
		version,
		saveVersion,
		gameName,
		organizationName,
		loggingEnabled,
		logToConsole,
		logToFile,
		runTests,
		saveOnClose,
		saveReadableStoreFiles,
		saveWholeSceneOnClose,
		hydrateOnLoad,
		launchToLastScene,
		storageLocation,
		visualizeContainers,
		visualizeClipRects,
		useAssetPack,
		enableTestScenes,
		immediateDebugOutput
	};

	class GameConfig : public BaseConfig<Param>
	{
		std::unordered_map<std::string, std::string> injectedConfig;
	public:
		GameConfig(const std::unordered_map<std::string, std::string>& injectedConfig) : injectedConfig(injectedConfig) {}

	protected:
		const std::map<Param, std::string> getConfig()
		{
			return std::map<Param, std::string>
			{
				{ Param::version, injectedConfig.at("version") },
				{ Param::saveVersion, injectedConfig.at("saveVersion") },
				{ Param::gameName, injectedConfig.at("gameName") },
				{ Param::organizationName, injectedConfig.at("organizationName") },
				{ Param::storageLocation, injectedConfig.at("storageLocation") },
				{ Param::loggingEnabled, injectedConfig.at("loggingEnabled") },
				{ Param::logToConsole, injectedConfig.at("logToConsole") },
				{ Param::logToFile, injectedConfig.at("logToFile") },
				{ Param::runTests, injectedConfig.at("runTests") },
				{ Param::saveOnClose, injectedConfig.at("saveOnClose") },
				{ Param::saveReadableStoreFiles, injectedConfig.at("saveReadableStoreFiles") },
				{ Param::saveWholeSceneOnClose, injectedConfig.at("saveWholeSceneOnClose") },
				{ Param::hydrateOnLoad, injectedConfig.at("hydrateOnLoad") },
				{ Param::launchToLastScene, injectedConfig.at("launchToLastScene") },
				{ Param::visualizeContainers, injectedConfig.at("visualizeContainers") },
				{ Param::visualizeClipRects, injectedConfig.at("visualizeClipRects") },
				{ Param::useAssetPack, injectedConfig.at("useAssetPack") },
				{ Param::enableTestScenes, injectedConfig.at("enableTestScenes") },
				{ Param::immediateDebugOutput, injectedConfig.at("immediateDebugOutput") }
			};
		}
	};

}
