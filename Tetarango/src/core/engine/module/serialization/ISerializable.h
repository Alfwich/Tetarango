#pragma once

#include <string>
#include <vector>
#include <map>
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
		Schematic(std::string name, std::shared_ptr<ISerializable>(*factory)()) { this->typeName = name; this->factory = factory; };
		std::shared_ptr<ISerializable>(*factory)();
		std::string typeName;
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
	public:
		ISerializable() {
			this->serializationClient = std::make_shared<SerializationClient>();
		};
		virtual ~ISerializable() = 0 {};
		std::shared_ptr<SerializationClient> serializationClient;

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
