#pragma once

#include "engine/module/IBaseModule.h"
#include "engine/module/filesystem/Filesystem.h"
#include "engine/module/storage/Storage.h"
#include "ResourceBundle.h"
#include "ImageBundle.h"

namespace AW
{
	class Asset : public IBaseModule
	{
		std::shared_ptr<Storage> storage;
		std::shared_ptr<Filesystem> filesystem;

		unsigned int cursorPosition = 0;
		bool hasLoadedGlobalAssetPack = false;

		const std::string assetPackStart = "##ASSET_PACK_START##";
		const std::string assetPackEnd = "##ASSET_PACK_END##";
		const std::string assetStart = "##ASSET_START##";
		const std::string assetEnd = "##ASSET_END##";
		const std::string binaryDelimiter = "#B";
		const std::string key = "sd6tSaIECaZqB2ZzpQIABjs4LnDrT89mzKq9S92jZbWQYsr8ZXNSXtsLphWSSgzx";

		std::map<std::string, std::shared_ptr<ResourceBundle>> assetPack;

		std::string getAssetNameFromRaw(char* data);
		std::unique_ptr<char[]> getAssetDataFromRaw(char* data, unsigned int& size, unsigned int& kPos);
		bool checkAndMoveIfCorrect(char* data, std::string tag);
		bool beginsWith(char* data, unsigned int pos, std::string tag);

		std::string assetNameFromPath(const std::string& path);

		void decodeAssetPack(const std::string& packName);

	public:
		void bindFilesystem(std::shared_ptr<Filesystem> filesystem);
		void bindStorage(std::shared_ptr<Storage> storage);

		void loadGlobalAssetPack();

		std::shared_ptr<ResourceBundle> getAssetBundle(std::string path, bool allowCached = true);

		void compressRawImageToPng(std::shared_ptr<ImageBundle> inBundle, std::shared_ptr<ImageBundle> outBundle);
		void uncompressPngImageToRaw(std::shared_ptr<ImageBundle> inBundle, std::shared_ptr<ImageBundle> outBundle);
	};

}
