#include "Hydrater.h"

namespace AW
{

	std::unordered_map<std::string, std::shared_ptr<Schematic>> Hydrater::schematics;
	std::unordered_map<std::string, std::shared_ptr<Schematic>> Hydrater::shortNameSchematics;

	bool Hydrater::hasSchematic(const std::string& name)
	{
		return Hydrater::schematics.count(name) == 1;
	}

	void Hydrater::registerSchematic(const std::shared_ptr<Schematic>& schematic)
	{
		if (Hydrater::schematics.count(schematic->typeName) == 1)
		{
			return;
		}

		Hydrater::schematics[schematic->typeName] = schematic;

		if (Hydrater::shortNameSchematics.count(schematic->shortTypeName))
		{
			Logger::instance()->logFatal("Hydrater::Attempted to define multiple types for shortName=" + schematic->shortTypeName);
		}
		Hydrater::shortNameSchematics[schematic->shortTypeName] = schematic;
	}

	const std::shared_ptr<Schematic> Hydrater::getSchematic(const std::string& name)
	{
		if (Hydrater::schematics.count(name) == 0)
		{
			Logger::instance()->logCritical("Hydrater::Failed to find schematic for name=" + name);
			return nullptr;
		}

		return Hydrater::schematics[name];
	}

	const std::shared_ptr<Schematic> Hydrater::getShortNameSchematic(const std::string& name)
	{
		if (Hydrater::shortNameSchematics.count(name) == 0)
		{
			Logger::instance()->logCritical("Hydrater::Failed to find short name schematic for name=" + name);
			return nullptr;
		}

		return Hydrater::shortNameSchematics[name];
	}


	const std::unordered_map<std::string, std::shared_ptr<Schematic>>& Hydrater::getAllSchemeatics()
	{
		return Hydrater::schematics;
	}

	Hydrater::Hydrater(const std::string& data)
		: data(data)
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

		Logger::instance()->logCritical("Hydrater::Found corrupt data at position=" + std::to_string(cursorPosition) + ", looking for tag=" + tag);
		return false;
	}

	std::string Hydrater::getTypeName()
	{
		unsigned int endOfTypeNamePos = StringHelper::distanceToLeft_offset(data, cursorPosition, SerializationTags::TYPE_NAME_END_TAG);

		return StringHelper::getSliceOfCharVector(data, cursorPosition, endOfTypeNamePos);
	}

	void Hydrater::getPramMap(std::unordered_map < std::string, std::string>& inMap)
	{
		while (!StringHelper::startsWith_Offset(data, cursorPosition, SerializationTags::PRAM_END_TAG))
		{
			std::string pramName = getPramName();
			std::string value = getPramValue();
			inMap[pramName] = value;
		}
		moveCursor(SerializationTags::PRAM_END_TAG);
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
		Logger::instance()->logCritical("Hydrater::Found corrupt data at position=" + std::to_string(cursorPosition));;
	}

	std::shared_ptr<ISerializable> Hydrater::hydrateData()
	{
		cursorPosition = 0;
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

		auto params = std::unordered_map<std::string, std::string>();
		getPramMap(params);
		ser->addData(params);
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
