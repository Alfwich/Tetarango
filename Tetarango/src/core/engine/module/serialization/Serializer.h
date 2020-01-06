#pragma once

#include "ISerializable.h"
#include "SerializationTags.h"

namespace AW
{

	class Serializer
	{
		std::unordered_map<std::string, std::string> data;
		std::vector<std::string> childrenData;
		std::string typeName;
		std::string getSerializedData();
		std::string serializeRecursive(std::shared_ptr<ISerializable> ser);

		bool isHumanReadable = false;
		int indentDepth = 0;

		std::string getTag(const char* tag);

	public:
		Serializer() {};
		Serializer(bool humanReadable)
		{
			isHumanReadable = humanReadable;
		};

		void addChildData(std::string childData);

		std::string serialize(std::shared_ptr<ISerializable> ser);
	};

}
