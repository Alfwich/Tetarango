#include "Serializer.h"

namespace AW
{
	void Serializer::addChildData(std::string childData)
	{
		childrenData.push_back(childData);
	}

	std::string Serializer::getSerializedData()
	{
		std::stringstream ss;
		ss << getTag(SerializationTags::OBJECT_START_TAG)
			<< getTag(SerializationTags::TYPE_NAME_START_TAG)
			<< typeName
			<< getTag(SerializationTags::TYPE_NAME_END_TAG)
			<< getTag(SerializationTags::PRAM_START_TAG);

		for (const auto prop : data)
		{
			if (prop.second.empty())
			{
				continue;
			}

			ss << prop.first
				<< getTag(SerializationTags::PRAM_VALUE_TAG)
				<< prop.second
				<< getTag(SerializationTags::PRAM_DELIM_TAG);
		}
		ss << getTag(SerializationTags::PRAM_END_TAG)
			<< getTag(SerializationTags::CHILDREN_LIST_START);

		for (const auto child : childrenData)
		{
			ss << child
				<< getTag(SerializationTags::CHILDREN_LIST_DELIM);
		}
		ss << getTag(SerializationTags::CHILDREN_LIST_END)
			<< getTag(SerializationTags::OBJECT_END_TAG);

		return ss.str();
	}

	std::string Serializer::serializeRecursive(std::shared_ptr<ISerializable> ser)
	{
		if (!ser->shouldSerializeSelf())
		{
			return std::string();
		}

		auto serializer = std::make_shared<Serializer>();
		auto schematic = ser->getSchematic();
		serializer->typeName = schematic->typeName;

		serializer->data = ser->doSerialize(SerializationHint::SERIALIZE)->getData();

		if (ser->shouldSerializeChildren())
		{
			for (const auto child : ser->getSerializableChildren())
			{
				++indentDepth;
				std::string childData = serializeRecursive(child);
				--indentDepth;

				if (childData.size())
				{
					serializer->addChildData(childData);
				}
			}
		}

		return serializer->getSerializedData();
	}

	std::string Serializer::getTag(const char* tag)
	{
		if (isHumanReadable)
		{
			return StringHelper::convertAndPad(std::string(tag), indentDepth, ' ') + "\n";
		}

		return std::string(tag);
	}

	std::string Serializer::serialize(std::shared_ptr<ISerializable> ser)
	{
		return SerializationTags::ARCHIVE_START_TAG + serializeRecursive(ser) + SerializationTags::ARCHIVE_END_TAG;
	}

}
