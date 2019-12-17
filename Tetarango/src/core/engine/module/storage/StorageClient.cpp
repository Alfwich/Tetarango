#include "StorageClient.h"
#include "engine/module/logger/Logger.h"


namespace AW
{

	StorageClient::StorageClient(std::unordered_map<std::string, std::string>& store, std::string scopeName) : store(store)
	{
		name = scopeName;
	}

	std::string StorageClient::getScopeName()
	{
		return name;
	}

	bool StorageClient::hasKey(std::string key)
	{
		return store.count(key) == 1;
	}

	int StorageClient::numberOfKeys()
	{
		return (int)store.size();
	}

	void StorageClient::writeString(std::string key, std::string value)
	{
		store[key] = value;
	}

	std::string StorageClient::readSring(std::string key)
	{
		if (store.count(key) == 0)
		{
			Logger::instance()->logCritical("StorageReader::Attempted to read key=" + key + ", this does not exist");
			return std::string();
		}

		return store[key];
	}

	void StorageClient::writeInt(std::string key, int value)
	{
		writeString(key, std::to_string(value));
	}

	int StorageClient::readInt(std::string key)
	{
		const auto value = readSring(key);
		return value.empty() ? 0 : std::stoi(value);
	}

	void StorageClient::writeDouble(std::string key, double value)
	{
		writeString(key, std::to_string(value));
	}

	double StorageClient::readDouble(std::string key)
	{
		const auto value = readSring(key);
		return value.empty() ? 0.0 : std::stod(value);
	}

	void StorageClient::writeBool(std::string key, bool value)
	{
		writeString(key, std::to_string(value));
	}

	bool StorageClient::readBool(std::string key)
	{
		const auto value = readSring(key);
		return value != "" && std::stoi(value) != 0;
	}

	const std::unordered_map<std::string, std::string> StorageClient::getStore()
	{
		return store;
	}
}
