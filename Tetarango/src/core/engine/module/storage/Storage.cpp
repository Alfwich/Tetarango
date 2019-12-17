#include "Storage.h"

namespace
{
	const auto dataFileExtension = ".gdata";
}

namespace AW
{
	std::string Storage::serializeStore(std::shared_ptr<StorageClient> client, bool isHumanReadable)
	{
		std::stringstream ss;
		int depth = 0;

		ss << getTag(archiveStartTag, depth, isHumanReadable);
		ss << getTag(archiveVersionStartTag, depth, isHumanReadable);
		ss << gameConfig->getConfigString(Config::Param::saveVersion);
		ss << getTag(archiveVersionEndTag, depth, isHumanReadable);
		++depth;
		for (const auto keyValue : client->getStore())
		{
			ss << getTag(keyValue.first, depth, isHumanReadable)
				<< getTag(keyValueDelimiter, depth, isHumanReadable)
				<< getTag(keyValue.second, depth, isHumanReadable)
				<< getTag(propertyDelimiter, depth, isHumanReadable);
		}
		--depth;
		ss << getTag(archiveEndTag, depth, isHumanReadable);

		return ss.str();
	}

	std::string Storage::getTag(std::string tag, int depth, bool isHumanReadable)
	{
		if (isHumanReadable)
		{
			if (tag == archiveStartTag || tag == archiveEndTag)
			{
				return std::string();
			}
			else if (tag == keyValueDelimiter)
			{
				return "=";
			}
			else if (tag == propertyDelimiter)
			{
				return "\r\n";
			}

			return StringHelper::convertAndPad(std::string(tag), depth, ' ');
		}

		return std::string(tag);
	}

	void Storage::hydrateStore(std::shared_ptr<StorageClient> client, std::string serializedData)
	{
		if (serializedData.size() == 0)
		{
			return;
		}

		cursorPosition = 0;

		if (!checkAndMoveIfCorrect(&serializedData, archiveStartTag, 0))
		{
			return;
		}

		if (!checkAndMoveIfCorrect(&serializedData, archiveVersionStartTag, 0))
		{
			return;
		}

		const auto archiveVersion = getVersionFromRaw(&serializedData);
		const auto expectedVersion = gameConfig->getConfigString(Config::Param::saveVersion);
		if (archiveVersion != expectedVersion)
		{
			// TODO: Convert old save files to new format if needed
			Logger::instance()->logCritical("Storage::Found different save file version: " + archiveVersion + " than what was expected: " + expectedVersion);
			return;
		}

		while (!StringHelper::startsWith_Offset(&serializedData, cursorPosition, archiveEndTag))
		{
			const std::string key = getKeyFromRaw(&serializedData);
			const std::string value = getValueFromRaw(&serializedData);

			if (key.size() == 0)
			{
				Logger::instance()->logCritical("Storage::Found empty key with value=" + value + ", ignoring");
				continue;
			}

			if (client->hasKey(key))
			{
				Logger::instance()->logCritical("Storage::Clobbering data in store with key= " + key + " which had value: " + client->readSring(key));
			}

			client->writeString(key, value);
		}
	}

	std::unordered_map<std::string, std::string> Storage::hydrateRaw(std::string& serializedData)
	{
		std::unordered_map<std::string, std::string> result;

		if (serializedData.size() == 0)
		{
			return result;
		}

		cursorPosition = 0;

		if (!checkAndMoveIfCorrect(&serializedData, archiveStartTag, 0))
		{
			return result;
		}

		if (!checkAndMoveIfCorrect(&serializedData, archiveVersionStartTag, 0))
		{
			return result;
		}

		const auto archiveVersion = getVersionFromRaw(&serializedData);
		const auto expectedVersion = gameConfig->getConfigString(Config::Param::saveVersion);
		if (archiveVersion != expectedVersion)
		{
			// TODO: Convert old save files to new format if needed
			Logger::instance()->logCritical("Storage::Found different save file version: " + archiveVersion + " than what was expected: " + expectedVersion);
			return result;
		}

		while (!StringHelper::startsWith_Offset(&serializedData, cursorPosition, archiveEndTag))
		{
			const std::string key = getKeyFromRaw(&serializedData);
			const std::string value = getValueFromRaw(&serializedData);

			if (key.size() == 0)
			{
				Logger::instance()->logCritical("Storage::Found empty key with value=" + value + ", ignoring");
				continue;
			}

			if (result.count(key) == 1)
			{
				Logger::instance()->logCritical("Storage::Clobbering data in store with key= " + key + " which had value: " + result[key]);
			}

			result[key] = value;
		}

		return result;
	}


	bool Storage::checkAndMoveIfCorrect(std::string* data, std::string tag, int additionalOffset)
	{
		if (StringHelper::startsWith_Offset(data, cursorPosition, tag))
		{
			cursorPosition += (int)tag.size() + additionalOffset;
			return true;
		}

		Logger::instance()->logCritical("Storage::Found corrupt data at position: " + std::to_string(cursorPosition) + ", looking for tag: " + tag);
		return false;
	}

	std::string Storage::getVersionFromRaw(std::string* data)
	{
		unsigned int endOfKeyPos = StringHelper::distanceToLeft_offset(data, cursorPosition, archiveVersionEndTag);
		std::string version = StringHelper::getSliceOfCharVector(data, cursorPosition, endOfKeyPos);

		cursorPosition += (int)archiveVersionEndTag.size() + endOfKeyPos;

		return version;

	}

	std::string Storage::getKeyFromRaw(std::string* data)
	{
		unsigned int endOfKeyPos = StringHelper::distanceToLeft_offset(data, cursorPosition, keyValueDelimiter);
		std::string key = StringHelper::getSliceOfCharVector(data, cursorPosition, endOfKeyPos);

		cursorPosition += (int)keyValueDelimiter.size() + endOfKeyPos;

		return key;
	}

	std::string Storage::getValueFromRaw(std::string* data)
	{
		unsigned int endOfValuePos = StringHelper::distanceToLeft_offset(data, cursorPosition, propertyDelimiter);
		std::string key = StringHelper::getSliceOfCharVector(data, cursorPosition, endOfValuePos);

		cursorPosition += (int)propertyDelimiter.size() + endOfValuePos;

		return key;
	}

	void Storage::loadScope(std::shared_ptr<StorageClient> client)
	{
		auto savedData = filesystem->readContentsFromFile(client->getScopeName() + dataFileExtension);

		if (!savedData.empty())
		{
			decompress(savedData);
			hydrateStore(client, savedData);
		}

		Logger::instance()->log("Storage::Loaded scopeName=" + client->getScopeName() + " with " + std::to_string(client->numberOfKeys()) + " keys");
	}

	std::shared_ptr<StorageClient> Storage::getClient(std::string scopeName)
	{
		if (scopeName.empty())
		{
			scopeName = globalDataScopeName;
		}

		bool shouldLoadClient = dataScopes.count(scopeName);
		const auto client = std::make_shared<StorageClient>(dataScopes[scopeName], scopeName);

		if (!shouldLoadClient)
		{
			loadScope(client);
		}

		return client;
	}

	void Storage::loadDataScope(std::string scopeName)
	{
		getClient(scopeName);
	}

	WORKER_ID Storage::loadDataScopeAsync(std::string scopeName, std::weak_ptr<INotifyOnCompletion> callback)
	{
		if (thread->isCleanedUp)
		{
			return -1;
		}

		std::shared_ptr<Storage> thisServicePtr = std::static_pointer_cast<Storage>(shared_from_this());
		std::shared_ptr<Filesystem> filesystemPtr = filesystem;
		auto pair = std::make_pair(thisServicePtr, filesystemPtr);
		auto pairPtr = std::make_shared<std::pair<std::shared_ptr<Storage>, std::shared_ptr<Filesystem>>>(pair);
		auto bundle = std::make_shared<AsyncOperationBundle<std::pair<std::shared_ptr<Storage>, std::shared_ptr<Filesystem>>, std::string>>(pairPtr, std::make_shared<std::string>(scopeName));

		const auto workerId = thread->doWorkSharedPtr<std::unordered_map<std::string, std::string>, AsyncOperationBundle<std::pair<std::shared_ptr<Storage>, std::shared_ptr<Filesystem>>, std::string>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<std::pair<std::shared_ptr<Storage>, std::shared_ptr<Filesystem>>, std::string>> bundle) -> std::shared_ptr<std::unordered_map<std::string, std::string>> {
			const auto storage = bundle->service->first;
			const auto filesystem = bundle->service->second;
			auto readResult = filesystem->readContentsFromFile((*bundle->data) + dataFileExtension);
			return storage->hydrateRawDataToRawScope(readResult);
		}
		, weak_from_this(), WorkerTaskCode::STORE_LOAD_DATA);

		asyncDataScopes[workerId] = std::make_pair(scopeName, callback);

		return workerId;
	}

	void Storage::cleanupScope(std::string scopeName)
	{
		saveScope(scopeName);
		dataScopes.erase(scopeName);
	}

	WORKER_ID Storage::cleanupScopeAsync(std::string scopeName, std::weak_ptr<INotifyOnCompletion> callback)
	{
		if (thread->isCleanedUp)
		{
			return -1;
		}

		std::shared_ptr<Storage> thisServicePtr = std::static_pointer_cast<Storage>(shared_from_this());
		auto bundle = std::make_shared<AsyncOperationBundle<Storage, std::string>>(thisServicePtr, std::make_shared<std::string>(scopeName));

		const auto workerId = thread->doWorkSharedPtr<void, AsyncOperationBundle<Storage, std::string>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<Storage, std::string>> bundle) -> std::shared_ptr<void> {

			const auto storage = bundle->service;
			storage->cleanupScope((*bundle->data));
			return nullptr;
		}
		, weak_from_this(), WorkerTaskCode::STORE_CLEANUP_SCOPE);

		asyncDataScopes[workerId] = std::make_pair(scopeName, callback);

		return workerId;
	}

	void Storage::saveScope(std::string scopeName)
	{
		const auto client = getClient(scopeName);
		if (getClient(scopeName)->numberOfKeys() == 0)
		{
			return;
		}

		auto dataToStore = serializeStore(client);

		compress(dataToStore);

		filesystem->writeContentToFile(client->getScopeName() + dataFileExtension, dataToStore);

		if (gameConfig->getConfigBool(Config::Param::saveReadableStoreFiles))
		{
			const auto readableData = serializeStore(client, true);
			filesystem->writeContentToFile(client->getScopeName() + dataFileExtension + ".txt", readableData);
		}

		Logger::instance()->log("Storage::Saved scopeName=" + client->getScopeName() + " to disk");
	}

	WORKER_ID Storage::saveScopeAsync(std::string scopeName, std::weak_ptr<INotifyOnCompletion> callback)
	{
		if (thread->isCleanedUp)
		{
			return -1;
		}

		std::shared_ptr<Storage> thisServicePtr = std::static_pointer_cast<Storage>(shared_from_this());
		auto bundle = std::make_shared<AsyncOperationBundle<Storage, std::string>>(thisServicePtr, std::make_shared<std::string>(scopeName));

		const auto workerId = thread->doWorkSharedPtr<void, AsyncOperationBundle<Storage, std::string>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<Storage, std::string>> bundle) -> std::shared_ptr<void> {

			const auto storage = bundle->service;
			storage->saveScope((*bundle->data));
			return nullptr;
		}
		, weak_from_this(), WorkerTaskCode::STORE_SAVE_SCOPE);

		asyncDataScopes[workerId] = std::make_pair(scopeName, callback);

		return workerId;
	}

	void Storage::onWorkError(WORKER_ID workerId, WorkerTaskCode code)
	{
		Logger::instance()->log("Worker failed id=" + std::to_string(workerId));

		const auto scopeCallbackPair = asyncDataScopes[workerId];

		const auto callbackPtr = scopeCallbackPair.second.lock();

		if (callbackPtr)
		{
			callbackPtr->onWorkError(workerId, code);
		}
	}

	void Storage::onWorkDone(WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<AsyncResultBundle> result)
	{
		switch (code)
		{
		case WorkerTaskCode::STORE_LOAD_DATA:
		{
			const auto scopeCallbackPair = asyncDataScopes[workerId];
			dataScopes[scopeCallbackPair.first] = *result->getResult<std::unordered_map<std::string, std::string>>();

			const auto callbackPtr = scopeCallbackPair.second.lock();

			if (callbackPtr)
			{
				callbackPtr->onWorkDone(workerId, code, nullptr);
			}

			asyncDataScopes.erase(workerId);
		}
		break;

		case WorkerTaskCode::STORE_SAVE_SCOPE:
		case WorkerTaskCode::STORE_CLEANUP_SCOPE:
		{
			const auto scopeCallbackPair = asyncDataScopes[workerId];
			const auto callbackPtr = scopeCallbackPair.second.lock();

			if (callbackPtr)
			{
				callbackPtr->onWorkDone(workerId, code, nullptr);
			}

			asyncDataScopes.erase(workerId);
		}
		break;
		}
	}

	void Storage::compress(std::string& input)
	{
		xorEncode(input);
	}

	void Storage::decompress(std::string& input)
	{
		xorDecode(input);
	}

	void Storage::bindFilesystem(std::shared_ptr<Filesystem> filesystem)
	{
		this->filesystem = filesystem;
	}

	void Storage::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	std::shared_ptr<std::unordered_map<std::string, std::string>> Storage::hydrateRawDataToRawScope(std::string& rawData)
	{
		decompress(rawData);
		return std::make_shared<std::unordered_map<std::string, std::string>>(hydrateRaw(rawData));
	}

	void Storage::xorEncode(std::string& str)
	{
		unsigned int i = 0, p = 0;
		while (i < str.size())
		{
			str[i] = str[i] ^ key[p++ % key.size()];
			++i;
		}
	}

	void Storage::xorDecode(std::string& str)
	{
		xorEncode(str);
	}

	void Storage::onInit()
	{
		// Loads the global store
		getClient();
	}

	void Storage::onCleanup()
	{
		if (gameConfig->getConfigBool(Config::Param::saveOnClose))
		{
			std::vector<std::string> scopesToCleanup;
			for (const auto scopeNameToData : dataScopes)
			{
				if (!scopeNameToData.second.empty())
				{
					scopesToCleanup.push_back(scopeNameToData.first);
				}
			}

			for (const auto scopeNameToCleanup : scopesToCleanup)
			{
				cleanupScope(scopeNameToCleanup);
			}
		}
	}
}
