#pragma once

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

		std::string getValue(const std::string& name);
		void setValue(const std::string& name, const std::string& value);
	public:
		SerializationClient();
		SerializationClient(const std::string& scope);
		SerializationClient(const std::string& scope, SerializationHint hint);

		std::shared_ptr<SerializationClient> getClient(const std::string& scope, SerializationHint hint = SerializationHint::UNSPECIFIED);
		std::string serializeString(const std::string& name, const std::string& value);
		double serializeDouble(const std::string& name, double value);
		int serializeInt(const std::string& name, int value);
		bool serializeBool(const std::string& name, bool value);

		std::string getString(const std::string& name, const std::string& defaultValue = "");
		double getDouble(const std::string& name, double defaultValue = 0.0);
		int getInt(const std::string& name, int defaultValue = 0);
		bool getBool(const std::string& name, bool defaultValue = false);

		void setString(const std::string& name, const std::string& value);
		void setDouble(const std::string& name, double value);
		void setInt(const std::string& name, int value);
		void setBool(const std::string& name, bool value);

		void addData(const std::unordered_map<std::string, std::string>& data);
		const std::unordered_map<std::string, std::string>& getData();
	};
}
