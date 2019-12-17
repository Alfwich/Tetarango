#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include "util/Rect.h"

namespace AW
{
	enum class SerializationHint
	{
		UNSPECIFIED,
		SERIALIZE,
		HYDRATE
	};

	class SerializationClient : std::enable_shared_from_this<SerializationClient>
	{
		std::unordered_map<std::string, std::string> data;

		std::string scope = "global";
		SerializationHint hint = SerializationHint::UNSPECIFIED;
		SerializationClient* parentClient = nullptr;
		void bindClient(SerializationClient* parentClient);

		std::string getValue(std::string name);
		void setValue(std::string name, std::string value);
	public:
		SerializationClient();
		SerializationClient(std::string scope);
		SerializationClient(std::string scope, SerializationHint hint);

		std::shared_ptr<SerializationClient> getClient(std::string scope, SerializationHint hint = SerializationHint::UNSPECIFIED);
		std::string serializeString(std::string name, std::string value);
		double serializeDouble(std::string name, double value);
		int serializeInt(std::string name, int value);
		bool serializeBool(std::string name, bool value);

		std::string getString(std::string name, std::string defaultValue = "");
		double getDouble(std::string name, double defaultValue = 0.0);
		int getInt(std::string name, int defaultValue = 0);
		bool getBool(std::string name, bool defaultValue = false);

		void setString(std::string name, std::string value);
		void setDouble(std::string name, double value);
		void setInt(std::string name, int value);
		void setBool(std::string name, bool value);

		/*
		std::string bindString(std::string name, std::string defaultValue);
		double bindDouble(std::string name, double defaultValue);
		int bindInt(std::string name, int defaultValue);
		bool bindBool(std::string name, bool defaultValue);
		*/

		void addData(const std::unordered_map<std::string, std::string>& data);
		const std::unordered_map<std::string, std::string>& getData();
	};
}
