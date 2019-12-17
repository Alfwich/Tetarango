#include "SerializationClient.h"

#include "SerializationTags.h"

namespace AW
{
	void SerializationClient::bindClient(SerializationClient* parentClient)
	{
		this->parentClient = parentClient;
	}

	std::string SerializationClient::getValue(std::string name)
	{
		if (parentClient != nullptr)
		{
			return parentClient->getValue(scope + name);
		}

		return data[scope + name];
	}

	void SerializationClient::setValue(std::string name, std::string value)
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

	SerializationClient::SerializationClient(std::string scope) : SerializationClient(scope, SerializationHint::UNSPECIFIED)
	{
	}

	SerializationClient::SerializationClient(std::string scope, SerializationHint hint)
	{
		this->scope = scope;
		this->hint = hint;
	}

	std::shared_ptr<SerializationClient> SerializationClient::getClient(std::string scope, SerializationHint hint)
	{
		const auto client = std::make_shared<SerializationClient>(scope, hint);
		client->bindClient(this);
		return client;
	}

	std::string SerializationClient::serializeString(std::string name, std::string value)
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

	double SerializationClient::serializeDouble(std::string name, double value)
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

	int SerializationClient::serializeInt(std::string name, int value)
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

	bool SerializationClient::serializeBool(std::string name, bool value)
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

	std::string SerializationClient::getString(std::string name, std::string defaultValue)
	{
		const auto result = getValue(name);
		return result.empty() ? defaultValue : result;
	}

	double SerializationClient::getDouble(std::string name, double defaultValue)
	{
		const auto myValue = getValue(name);
		return myValue.empty() ? defaultValue : std::stod(myValue);
	}

	int SerializationClient::getInt(std::string name, int defaultValue)
	{
		const auto myValue = getValue(name);
		return myValue.empty() ? defaultValue : std::stoi(myValue);
	}

	bool SerializationClient::getBool(std::string name, bool defaultValue)
	{
		const auto myValue = getValue(name);
		return myValue.empty() ? defaultValue : myValue == SerializationTags::TYPE_BOOL_TRUE;
	}

	void SerializationClient::setString(std::string name, std::string value)
	{
		setValue(name, value);
	}

	void SerializationClient::setDouble(std::string name, double value)
	{
		setValue(name, std::to_string(value));
	}

	void SerializationClient::setInt(std::string name, int value)
	{
		setValue(name, std::to_string(value));
	}

	void SerializationClient::setBool(std::string name, bool value)
	{
		setValue(name, value ? SerializationTags::TYPE_BOOL_TRUE : SerializationTags::TYPE_BOOL_FALSE);
	}

	/*
	std::string SerializationClient::bindString(std::string name, std::string defaultValue)
	{
		const auto myValue = getValue(name);

		if (myValue.empty())
		{
			setValue(name, defaultValue);
		}

		return myValue.empty() ? defaultValue : myValue;
	}

	double SerializationClient::bindDouble(std::string name, double defaultValue)
	{
		const auto myValue = getValue(name);

		if (myValue.empty())
		{
			setValue(name, std::to_string(defaultValue));
		}

		return myValue.empty() ? defaultValue : std::stod(myValue);
	}

	int SerializationClient::bindInt(std::string name, int defaultValue)
	{
		const auto myValue = getValue(name);

		if (myValue.empty())
		{
			setValue(name, std::to_string(defaultValue));
		}

		return myValue.empty() ? defaultValue : std::stoi(myValue);
	}

	bool SerializationClient::bindBool(std::string name, bool defaultValue)
	{
		const auto myValue = getValue(name);

		if (myValue.empty())
		{
			setValue(name, defaultValue ? SerializationTags::TYPE_BOOL_TRUE : SerializationTags::TYPE_BOOL_FALSE);
		}

		return myValue.empty() ? defaultValue : myValue == SerializationTags::TYPE_BOOL_TRUE;
	}
	*/

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
