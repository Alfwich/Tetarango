#pragma once

#include "ISerializable.h"
#include "SerializationTags.h"

namespace AW
{

	class Hydrater
	{
		static std::unordered_map<std::string, std::shared_ptr<Schematic>> schematics;
		static std::unordered_map<std::string, std::shared_ptr<Schematic>> shortNameSchematics;

		unsigned int cursorPosition;
		const std::string& data;

		std::shared_ptr<ISerializable> hydrateDataRecursive();
		std::shared_ptr<ISerializable> createObjectForType(std::string typeName);

		void moveCursor(const char* tag);
		void moveCursor(unsigned int x);
		bool checkAndMoveIfCorrect(const char* tag, int additionalOffset = 0);
		std::string getTypeName();

		void getPramMap(std::unordered_map<std::string, std::string>& inMap);
		std::string getPramName();
		std::string getPramValue();

		void hydrateChildren(std::shared_ptr<ISerializable> parent);
		void reportError();

	public:
		static bool hasSchematic(const std::string& name);
		static void registerSchematic(const std::shared_ptr<Schematic>& schematic);
		static const std::shared_ptr<Schematic> getSchematic(const std::string& name);
		static const std::shared_ptr<Schematic> getShortNameSchematic(const std::string& name);
		static const std::unordered_map<std::string, std::shared_ptr<Schematic>>& getAllSchemeatics();

		Hydrater(const std::string& data);

		std::shared_ptr<ISerializable> hydrateData();
	};

}
