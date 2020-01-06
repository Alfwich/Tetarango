#pragma once

namespace AW
{

	class StorageClient
	{
		std::unordered_map<std::string, std::string>& store;
		std::string name;

	public:
		StorageClient(std::unordered_map<std::string, std::string>& store, std::string scopeName);

		std::string getScopeName();
		bool hasKey(std::string key);
		int numberOfKeys();

		void writeString(std::string key, std::string value);
		std::string readSring(std::string key);
		void writeInt(std::string key, int value);
		int readInt(std::string key);
		void writeDouble(std::string key, double value);
		double readDouble(std::string key);
		void writeBool(std::string key, bool value);
		bool readBool(std::string key);

		const std::unordered_map<std::string, std::string> getStore();
	};

}
