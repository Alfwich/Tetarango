#pragma once

#include "ISerializable.h"
#include "SerializationTags.h"

namespace AW
{

	class Hydrater
	{
		static std::unordered_map<std::string, std::shared_ptr<Schematic>> schematics;

		unsigned int cursorPosition;
		std::string* data;

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
		static bool hasSchematic(std::string name);
		static void registerSchematic(std::shared_ptr<Schematic> schematic);
		static std::shared_ptr<Schematic> getSchematic(std::string name);

		Hydrater();

		std::shared_ptr<ISerializable> hydrateData(std::string data);
	};

}
