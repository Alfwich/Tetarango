#pragma once

#include <string>
#include <map>
#include "BaseConfig.h"

namespace Config
{
	enum class Param
	{
		version,
		saveVersion,
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
		visualizeContainers,
		visualizeClipRects,
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
					{ Param::version, "v0.07" },
					{ Param::saveVersion, "v1.00" },
					{ Param::gameName, "Tetarango" },
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
					{ Param::visualizeContainers, FALSE },
					{ Param::visualizeClipRects, FALSE },
					{ Param::useAssetPack, TRUE },
					{ Param::assetPackName, "asset.pack" },
					{ Param::enableTestScenes, FALSE },
			};

			base[Param::storageLocation] = base[Param::gameName];

			if (GAME_ENVIRONMENT == "DEV" || GAME_ENVIRONMENT == "PROD_DEBUG")
			{
				base[Param::gameName] = base[Param::gameName] + " - DEV - " + base[Param::version];
				base[Param::storageLocation] += " " + base[Param::version] + " - Debug";
				base[Param::launchToLastScene] = FALSE;
				base[Param::useAssetPack] = FALSE;
				base[Param::saveWholeSceneOnClose] = TRUE;
				//base[Param::visualizeContainers] = TRUE;
				//base[Param::visualizeClipRects] = TRUE;
				base[Param::runTests] = TRUE;
				//base[Param::hydrateOnLoad] = TRUE;
				//base[Param::enableTestScenes] = TRUE;
				//base[Param::saveReadableStoreFiles] = TRUE;
			}
			else if (GAME_ENVIRONMENT == "PROD")
			{
				//base[Param::enableTestScenes] = TRUE;
				//base[ConfigParam::throwOnCriticalLog] = TRUE;
				//base[Param::visualizeZones] = TRUE;
			}
			else
			{
				throw "GameConfig::Environment not found for value=" + std::string(GAME_ENVIRONMENT);
			}

			return base;
		}
	};

}
