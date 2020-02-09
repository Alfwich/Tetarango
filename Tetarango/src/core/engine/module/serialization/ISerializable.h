#pragma once

#include "SerializationClient.h"

namespace AW
{

	class Serializer;
	class Hydrater;
	class ISerializable;

	class Schematic
	{
	public:
		Schematic() {};
		Schematic(std::string name, std::shared_ptr<ISerializable>(*factory)())
			: factory(factory), typeName(name), shortTypeName(StringHelper::toLower(name.substr(name.find_last_of(':') + 1)))
		{
		}

		std::shared_ptr<ISerializable>(*factory)();
		const std::string typeName;
		std::string shortTypeName;
	};

	class ISerializableDataSubscriber
	{
	public:
		virtual ~ISerializableDataSubscriber() {};
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient) { /* NO-OP */ };
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint) = 0;
	};

	class ISerializable : public ISerializableDataSubscriber
	{
	protected:
		std::shared_ptr<SerializationClient> serializationClient;

	public:
		ISerializable()
		{
			this->serializationClient = std::make_shared<SerializationClient>();
		};
		virtual ~ISerializable() = 0 {};

		void addData(const std::unordered_map<std::string, std::string>& data) { serializationClient->addData(data); }

		std::string typeName;
		virtual std::shared_ptr<Schematic> getSchematic() = 0;
		virtual std::vector<std::shared_ptr<ISerializable>> getSerializableChildren() = 0;
		virtual bool shouldSerializeChildren() = 0;
		virtual bool shouldSerializeSelf() = 0;
		virtual void childHydrated(std::shared_ptr<ISerializable> child) = 0;
	};
}

#include "Serializer.h"
#include "Hydrater.h"
