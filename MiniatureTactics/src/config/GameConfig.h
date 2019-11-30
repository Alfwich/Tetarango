#pragma once

#include <string>
#include <map>
#include "BaseConfig.h"

namespace Config
{
	enum class Param
	{
		version,
		gameName,
		organizationName,
		logToConsole,
		logToFile,
		throwOnCriticalLog,
		runTests,
		saveOnClose,
		saveReadableStoreFiles,
		saveWholeSceneOnClose,
		hydrateOnLoad,
		launchToLastScene,
		storageLocation,
		visualizeZones,
		useAssetPack,
		assetPackName,
		enableTestScenes
	};

	class GameConfig : public BaseConfig<Param>
	{
	public:

	protected:
		const std::map<Param, std::string> getConfig() {
			std::map<Param, std::string> base
			{
					{ Param::version, "v0.03" },
					{ Param::gameName, "Tetris" },
					{ Param::organizationName, "AW Games" },
					{ Param::logToConsole, TRUE },
					{ Param::logToFile, TRUE },
					{ Param::throwOnCriticalLog, FALSE },
					{ Param::runTests, FALSE },
					{ Param::saveOnClose, TRUE },
					{ Param::saveReadableStoreFiles, FALSE },
					{ Param::saveWholeSceneOnClose, FALSE },
					{ Param::hydrateOnLoad, TRUE },
					{ Param::launchToLastScene, FALSE },
					{ Param::visualizeZones, FALSE },
					{ Param::useAssetPack, TRUE },
					{ Param::assetPackName, "asset.pack" },
					{ Param::enableTestScenes, FALSE },
			};

			base[Param::storageLocation] = base[Param::gameName];

			if (MT_ENVIRONMENT == "DEV" || MT_ENVIRONMENT == "PROD_DEBUG")
			{
				base[Param::gameName] = base[Param::gameName] + " - DEV - " + base[Param::version];
				base[Param::runTests] = TRUE;
				base[Param::storageLocation] += " " + base[Param::version] + " - Debug";
				base[Param::launchToLastScene] = TRUE;
				base[Param::useAssetPack] = FALSE;
				base[Param::hydrateOnLoad] = FALSE;
				//base[Param::enableTestScenes] = TRUE;
				//base[Param::saveWholeSceneOnClose] = TRUE;
				//base[Param::saveReadableStoreFiles] = TRUE;
				//base[Param::visualizeZones] = TRUE;
			}
			else if (MT_ENVIRONMENT == "PROD")
			{
				//base[Param::enableTestScenes] = TRUE;
				//base[ConfigParam::throwOnCriticalLog] = TRUE;
				//base[Param::visualizeZones] = TRUE;
			}
			else
			{
				throw "GameConfig::Environment not found for value=" + std::string(MT_ENVIRONMENT);
			}

			return base;
		}
	};

}
