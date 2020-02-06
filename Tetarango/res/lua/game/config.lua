require("core/table-helper")

local TRUE = "true"
local FALSE = "false"
local ver = "v0.20"
local gName = "Tetarango"

local _base_config = 
{
	version = ver,
	saveVersion = "v1.00",
	gameName = gName,
	organizationName = "AW Games",
	logToConsole = TRUE,
	logToFile = TRUE,
	loggingEnabled = TRUE,
	runTests = FALSE,
	saveOnClose = TRUE,
	saveReadableStoreFiles = FALSE,
	saveWholeSceneOnClose = FALSE,
	hydrateOnLoad = TRUE,
	launchToLastScene = FALSE,
	storageLocation = gName,
	visualizeContainers = FALSE,
	visualizeClipRects = FALSE,
	useAssetPack = TRUE,
	enableTestScenes = FALSE,
	immediateDebugOutput = FALSE
}

dev_config = table_helper.copy(_base_config)
dev_config.gameName = gName .. " - Debug - " .. ver
dev_config.storageLocation = gName .. "-" .. ver .. "-Debug"
dev_config.useAssetPack = FALSE
dev_config.saveWholeSceneOnClose = TRUE
dev_config.immediateDebugOutput = TRUE
dev_config.launchToLastScene = TRUE
dev_config.enableTestScenes = TRUE
--dev_config.runTests = TRUE
--dev_config.saveReadableStoreFiles = TRUE
--dev_config.hydrateOnLoad = TRUE
--dev_config.visualizeContainers = TRUE
--dev_config.visualizeClipRects = TRUE

prod_config = table_helper.copy(_base_config)
prod_config.enableTestScenes = TRUE
prod_config.loggingEnabled = FALSE 
--prod_config.useAssetPack = FALSE
--prod_config.immediateDebugOutput = TRUE
--prod_config.launchToLastScene = TRUE
--prod_config.saveWholeSceneOnClose = TRUE
--prod_config.saveReadableStoreFiles = TRUE
--prod_config.hydrateOnLoad = TRUE
--prod_config.visualizeContainers = TRUE
--prod_config.visualizeClipRects = TRUE

