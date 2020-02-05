#include "SerializationClient.h"

#include "SerializationTags.h"

namespace AW
{
	void SerializationClient::bindClient(SerializationClient* parentClient)
	{
		this->parentClient = parentClient;
	}

	std::string SerializationClient::getValue(const std::string& name)
	{
		if (parentClient != nullptr)
		{
			return parentClient->getValue(scope + name);
		}

		if (data.count(scope + name))
		{
			return data[scope + name];
		}

		return "";
	}

	void SerializationClient::setValue(const std::string& name, const std::string& value)
	{
		if (parentClient != nullptr)
		{
			return parentClient->setValue(scope + name, value);
		}

		data[scope + name] = value;
	}

	SerializationClient::SerializationClient() : SerializationClient("", SerializationHint::UNSPECIFIED)
	{
	}

	SerializationClient::SerializationClient(const std::string& scope) : SerializationClient(scope, SerializationHint::UNSPECIFIED)
	{
	}

	SerializationClient::SerializationClient(const std::string& scope, SerializationHint hint)
	{
		this->scope = scope;
		this->hint = hint;
	}

	std::shared_ptr<SerializationClient> SerializationClient::getClient(const std::string& scope, SerializationHint hint)
	{
		const auto client = std::make_shared<SerializationClient>(scope, hint);
		client->bindClient(this);
		return client;
	}

	std::string SerializationClient::serializeString(const std::string& name, const std::string& value)
	{
		const auto myValue = getValue(name);

		switch (hint)
		{
		case SerializationHint::UNSPECIFIED:
		case SerializationHint::SERIALIZE:
			setValue(name, value);
			return value;
			break;

		case SerializationHint::HYDRATE:
			return myValue;
			break;
		}

		return std::string();
	}

	double SerializationClient::serializeDouble(const std::string& name, double value)
	{
		const auto myValue = getValue(name);

		switch (hint)
		{
		case SerializationHint::UNSPECIFIED:
		case SerializationHint::SERIALIZE:
			setValue(name, std::to_string(value));
			return value;
			break;

		case SerializationHint::HYDRATE:
			return myValue.empty() ? 0.0 : std::stod(myValue);
			break;
		}

		return 0.0;
	}

	int SerializationClient::serializeInt(const std::string& name, int value)
	{
		const auto myValue = getValue(name);

		switch (hint)
		{
		case SerializationHint::UNSPECIFIED:
		case SerializationHint::SERIALIZE:
			setValue(name, std::to_string(value));
			return value;
			break;

		case SerializationHint::HYDRATE:
			return myValue.empty() ? 0 : std::stoi(myValue);
			break;
		}

		return 0;
	}

	bool SerializationClient::serializeBool(const std::string& name, bool value)
	{
		const auto myValue = getValue(name);

		switch (hint)
		{
		case SerializationHint::UNSPECIFIED:
		case SerializationHint::SERIALIZE:
			setValue(name, value ? SerializationTags::TYPE_BOOL_TRUE : SerializationTags::TYPE_BOOL_FALSE);
			return value;
			break;

		case SerializationHint::HYDRATE:
			return myValue == SerializationTags::TYPE_BOOL_TRUE;
			break;
		}

		return false;
	}

	std::string SerializationClient::getString(const std::string& name, const std::string& defaultValue)
	{
		const auto result = getValue(name);
		return result.empty() ? defaultValue : result;
	}

	double SerializationClient::getDouble(const std::string& name, double defaultValue)
	{
		const auto myValue = getValue(name);
		return myValue.empty() ? defaultValue : std::stod(myValue);
	}

	int SerializationClient::getInt(const std::string& name, int defaultValue)
	{
		const auto myValue = getValue(name);
		return myValue.empty() ? defaultValue : std::stoi(myValue);
	}

	bool SerializationClient::getBool(const std::string& name, bool defaultValue)
	{
		const auto myValue = getValue(name);
		return myValue.empty() ? defaultValue : myValue == SerializationTags::TYPE_BOOL_TRUE;
	}

	void SerializationClient::setString(const std::string& name, const std::string& value)
	{
		setValue(name, value);
	}

	void SerializationClient::setDouble(const std::string& name, double value)
	{
		setValue(name, std::to_string(value));
	}

	void SerializationClient::setInt(const std::string& name, int value)
	{
		setValue(name, std::to_string(value));
	}

	void SerializationClient::setBool(const std::string& name, bool value)
	{
		setValue(name, value ? SerializationTags::TYPE_BOOL_TRUE : SerializationTags::TYPE_BOOL_FALSE);
	}

	void SerializationClient::addData(const std::unordered_map<std::string, std::string>& data)
	{
		for (const auto keyValue : data)
		{
			this->data[keyValue.first] = keyValue.second;
		}
	}

	const std::unordered_map<std::string, std::string>& SerializationClient::getData()
	{
		return data;
	}
}
