#include "SerializationTags.h"

namespace AWCore
{
	const char* SerializationTags::ARCHIVE_START_TAG = "##SCENE_ARCHIVE_START##";
	const char* SerializationTags::ARCHIVE_END_TAG = "##SCENE_ARCHIVE_END##";

	const char* SerializationTags::OBJECT_START_TAG = "##SCENE_OBJECT_START##";
	const char* SerializationTags::OBJECT_END_TAG = "##SCENE_OBJECT_END##";

	const char* SerializationTags::TYPE_NAME_START_TAG = "##SCENE_TYPE_NAME_START##";
	const char* SerializationTags::TYPE_NAME_END_TAG = "##SCENE_TYPE_NAME_END##";

	const char* SerializationTags::PRAM_START_TAG = "##SCENE_PRAM_START##";
	const char* SerializationTags::PRAM_VALUE_TAG = "##SCENE_PRAM_VALUE##";
	const char* SerializationTags::PRAM_DELIM_TAG = "##SCENE_PRAM_DELIM##";
	const char* SerializationTags::PRAM_END_TAG = "##SCENE_PRAM_END##";

	const char* SerializationTags::CHILDREN_LIST_START = "##SCENE_CHILDREN_LIST_DELIM##";
	const char* SerializationTags::CHILDREN_LIST_DELIM = "##SCENE_CHILDREN_LIST_START##";
	const char* SerializationTags::CHILDREN_LIST_END = "##SCENE_CHILDREN_LIST_END##";

	const char* SerializationTags::TYPE_BOOL_TRUE = "##SCENE_TYPE_BOOL_TRUE##";
	const char* SerializationTags::TYPE_BOOL_FALSE = "##SCENE_TYPE_BOOL_FALSE##";

}
