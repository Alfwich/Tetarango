#pragma once

#include <memory>
#include "WorkerStatus.h"

namespace AWCore
{
	typedef unsigned int WORKER_ID;
	class AsyncResultBundle;

	class INotifyOnCompletion
	{
	public:
		virtual void onWorkError(WORKER_ID workerId, WorkerTaskCode code) = 0;
		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<AsyncResultBundle> result) = 0;
	};
}
