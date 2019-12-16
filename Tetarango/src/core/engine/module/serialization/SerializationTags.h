#pragma once

namespace AWCore
{

	class SerializationTags
	{
	public:
		static const char* ARCHIVE_START_TAG;
		static const char* ARCHIVE_END_TAG;
		static const char* OBJECT_START_TAG;
		static const char* OBJECT_END_TAG;
		static const char* TYPE_NAME_START_TAG;
		static const char* TYPE_NAME_END_TAG;
		static const char* PRAM_START_TAG;
		static const char* PRAM_VALUE_TAG;
		static const char* PRAM_DELIM_TAG;
		static const char* PRAM_END_TAG;
		static const char* CHILDREN_LIST_START;
		static const char* CHILDREN_LIST_DELIM;
		static const char* CHILDREN_LIST_END;
		static const char* TYPE_BOOL_TRUE;
		static const char* TYPE_BOOL_FALSE;
	};

}
