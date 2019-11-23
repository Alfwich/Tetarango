#include "Hydrater.h"

namespace MT
{

	std::unordered_map<std::string, std::shared_ptr<Schematic>> Hydrater::schematics;

	bool Hydrater::hasSchematic(std::string name)
	{
		return Hydrater::schematics.count(name) == 1;
	}

	void Hydrater::registerSchematic(std::shared_ptr<Schematic> schematic)
	{
		if (Hydrater::schematics.count(schematic->typeName) == 1)
		{
			return;
		}

		Hydrater::schematics[schematic->typeName] = schematic;
	}

	std::shared_ptr<Schematic> Hydrater::getSchematic(std::string name)
	{
		if (Hydrater::schematics.count(name) == 0)
		{
			Logger::instance()->logCritical("Hydrater::Failed to find schematic for name=" + name);
			return nullptr;
		}

		return Hydrater::schematics[name];
	}

	Hydrater::Hydrater()
	{
		cursorPosition = 0;
	}

	std::shared_ptr<ISerializable> Hydrater::createObjectForType(std::string typeName)
	{
		if (Hydrater::schematics.count(typeName) != 1)
		{
			Logger::instance()->logCritical("Hydrater::Failed to create type for name=" + typeName + ", This will need to be registered for serialization to work.");
			return nullptr;
		}

		return Hydrater::schematics[typeName]->factory();
	}

	void Hydrater::moveCursor(const char* tag)
	{
		cursorPosition += (int)strlen(tag);
	}

	void Hydrater::moveCursor(unsigned int x)
	{
		cursorPosition += x;
	}

	bool Hydrater::checkAndMoveIfCorrect(const char* tag, int additionalOffset)
	{
		if (StringHelper::startsWith_Offset(data, cursorPosition, tag))
		{
			moveCursor((int)strlen(tag) + additionalOffset);
			return true;
		}

		Logger::instance()->logCritical("Hydrater::Found corrupt data at position: " + std::to_string(cursorPosition) + ", looking for tag: " + tag);
		return false;
	}

	std::string Hydrater::getTypeName()
	{
		unsigned int endOfTypeNamePos = StringHelper::distanceToLeft_offset(data, cursorPosition, SerializationTags::TYPE_NAME_END_TAG);

		return StringHelper::getSliceOfCharVector(data, cursorPosition, endOfTypeNamePos);
	}

	std::unordered_map<std::string, std::string> Hydrater::getPramMap()
	{
		auto map = std::unordered_map<std::string, std::string>();

		while (!StringHelper::startsWith_Offset(data, cursorPosition, SerializationTags::PRAM_END_TAG))
		{
			std::string pramName = getPramName();
			std::string value = getPramValue();
			map[pramName] = value;
		}
		moveCursor(SerializationTags::PRAM_END_TAG);

		return map;
	}

	std::string Hydrater::getPramName()
	{
		unsigned int pramNameSize = StringHelper::distanceToLeft_offset(data, cursorPosition, SerializationTags::PRAM_VALUE_TAG);
		std::string pramName = StringHelper::getSliceOfCharVector(data, cursorPosition, pramNameSize);

		moveCursor((int)strlen(SerializationTags::PRAM_VALUE_TAG) + pramNameSize);

		return pramName;
	}

	std::string Hydrater::getPramValue()
	{
		unsigned int valueSize = StringHelper::distanceToLeft_offset(data, cursorPosition, SerializationTags::PRAM_DELIM_TAG);
		std::string value = StringHelper::getSliceOfCharVector(data, cursorPosition, valueSize);

		moveCursor((int)strlen(SerializationTags::PRAM_DELIM_TAG) + valueSize);

		return value;
	}

	void Hydrater::hydrateChildren(std::shared_ptr<ISerializable> parent)
	{
		while (!StringHelper::startsWith_Offset(data, cursorPosition, SerializationTags::CHILDREN_LIST_END))
		{
			std::shared_ptr<ISerializable> child = hydrateDataRecursive();
			parent->childHydrated(child);
			moveCursor(SerializationTags::CHILDREN_LIST_DELIM);
		}

		moveCursor(SerializationTags::CHILDREN_LIST_END);
	}

	void Hydrater::reportError()
	{
		Logger::instance()->logCritical("Hydrater::Found corrupt data at position: " + std::to_string(cursorPosition));;
	}

	std::shared_ptr<ISerializable> Hydrater::hydrateData(std::string data)
	{
		cursorPosition = 0;
		this->data = &data;
		if (!checkAndMoveIfCorrect(SerializationTags::ARCHIVE_START_TAG))
		{
			reportError();
			return nullptr;
		}

		return hydrateDataRecursive();
	}

	std::shared_ptr<ISerializable> Hydrater::hydrateDataRecursive()
	{
		if (!checkAndMoveIfCorrect(SerializationTags::OBJECT_START_TAG))
		{
			reportError();
			return nullptr;
		}

		if (!checkAndMoveIfCorrect(SerializationTags::TYPE_NAME_START_TAG))
		{
			reportError();
			return nullptr;
		}

		std::string typeName = getTypeName();

		if (typeName.empty())
		{
			reportError();
			return nullptr;
		}
		moveCursor((int)strlen(SerializationTags::TYPE_NAME_END_TAG) + (int)typeName.size());

		std::shared_ptr<ISerializable> ser = createObjectForType(typeName);

		if (ser == nullptr)
		{
			return nullptr;
		}

		if (!checkAndMoveIfCorrect(SerializationTags::PRAM_START_TAG))
		{
			reportError();
			return nullptr;
		}

		std::unordered_map<std::string, std::string> pramValueBuffer = getPramMap();
		ser->serializationClient->addData(pramValueBuffer);
		ser->doSerialize(SerializationHint::HYDRATE);

		if (!checkAndMoveIfCorrect(SerializationTags::CHILDREN_LIST_START))
		{
			reportError();
			return nullptr;
		}

		hydrateChildren(ser);

		if (!checkAndMoveIfCorrect(SerializationTags::OBJECT_END_TAG))
		{
			reportError();
			return nullptr;
		}

		return ser;
	}
}
