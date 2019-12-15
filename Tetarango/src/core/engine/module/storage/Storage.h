#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include "engine/module/filesystem/Filesystem.h"
#include "engine/module/IBaseModule.h"
#include "util/StringHelper.h"
#include "HuffmanEncoder.h"
#include "StorageClient.h"

namespace MT
{
	class Storage : public IBaseModule
	{
		const std::string archiveStartTag = "##STORE_ARCHIVE_START##";
		const std::string archiveEndTag = "##STORE_ARCHIVE_END##";
		const std::string keyValueDelimiter = "##STORE_KEY_VALUE_DELIMITER##";
		const std::string propertyDelimiter = "##STORE_PROPERTY_DELIMITER##";
		const std::string archiveVersionStartTag = "##STORE_VERSION_START##";
		const std::string archiveVersionEndTag = "##STORE_VERSION_END##";

		const std::string key = "rRGT1JX2DlsWehGSmLh1WbC4J3KW6AVYed0MPg5umieBZ5Q3SMlk9CVCHWdw6RIu";

		const std::string globalDataScopeName = "global";

		std::shared_ptr<Filesystem> filesystem;
		std::shared_ptr<Thread> thread;

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> dataScopes;
		std::unordered_map<WORKER_ID, std::pair<std::string, std::weak_ptr<INotifyOnCompletion>>> asyncDataScopes;

		int cursorPosition;

		std::string serializeStore(std::shared_ptr<StorageClient> client, bool isHumanReadable = false);
		std::string getTag(std::string tag, int depth, bool isHumanReadable);
		void hydrateStore(std::shared_ptr<StorageClient> client, std::string serializedData);
		std::unordered_map<std::string, std::string> hydrateRaw(std::string& serializedData);

		bool checkAndMoveIfCorrect(std::string* data, std::string tag, int additionalOffset);
		std::string getVersionFromRaw(std::string* data);
		std::string getKeyFromRaw(std::string* data);
		std::string getValueFromRaw(std::string* data);

		void loadScope(std::shared_ptr<StorageClient> client);

		HuffmanEncoder encoder;

	public:
		void bindFilesystem(std::shared_ptr<Filesystem> filesystem);
		void bindThread(std::shared_ptr<Thread> thread);

		std::shared_ptr<std::unordered_map<std::string, std::string>> hydrateRawDataToRawScope(std::string& rawData);

		void loadDataScope(std::string scopeName);
		WORKER_ID loadDataScopeAsync(std::string scopeName, std::weak_ptr<INotifyOnCompletion> callback = std::weak_ptr<INotifyOnCompletion>());

		void cleanupScope(std::string scopeName);
		WORKER_ID cleanupScopeAsync(std::string scopeName, std::weak_ptr<INotifyOnCompletion> callback = std::weak_ptr<INotifyOnCompletion>());

		void saveScope(std::string scopeName);
		WORKER_ID saveScopeAsync(std::string scopeName, std::weak_ptr<INotifyOnCompletion> callback = std::weak_ptr<INotifyOnCompletion>());

		std::shared_ptr<StorageClient> getClient(std::string scopeName = "");

		void onWorkError(WORKER_ID workerId, WorkerTaskCode code);
		void onWorkDone(WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<AsyncResultBundle> result);

		void compress(std::string& input);
		void decompress(std::string& input);

		void xorEncode(std::string& str);
		void xorDecode(std::string& str);

		void onInit();
		void onCleanup();
	};
}

