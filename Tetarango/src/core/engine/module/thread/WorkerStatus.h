#pragma once

namespace AW
{
	enum class WorkerStatus
	{
		CLOSED,
		DONE,
		WORKING,
		ERROR,
		NOT_STARTED
	};

	enum class WorkerTaskCode
	{
		UNSPECIFIED,
		HYDRATION,
		SERIALIZATION,
		FILE_WRITE,
		FILE_READ,
		FILE_APPEND,
		STORE_LOAD_DATA,
		STORE_SAVE_SCOPE,
		STORE_CLEANUP_SCOPE
	};

}