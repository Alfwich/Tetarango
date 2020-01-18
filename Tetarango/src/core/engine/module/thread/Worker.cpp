#include "Worker.h"

namespace AW
{

	Worker::Worker(void* data, void* (*toDo)(void* data), std::weak_ptr<INotifyOnCompletion> callback, WORKER_ID workerId)
		: data(data), workerId(workerId), toDo(toDo), callback(callback), status(WorkerStatus::NOT_STARTED)
	{
	}

	WORKER_ID Worker::getId()
	{
		return workerId;
	}

	void Worker::doWork()
	{
		if (data != nullptr)
		{
			result = toDo(data);
			status = WorkerStatus::DONE;
			data = nullptr;
		}
		else
		{
			status = WorkerStatus::ERROR;
		}

		resultBundle = std::make_shared<AsyncResultBundle>(status, taskCode, result);
	}

	void* Worker::getResult()
	{
		return result;
	}

	void Worker::doCallback()
	{
		const auto ptr = callback.lock();
		if (resultBundle == nullptr || ptr == nullptr)
		{
			return;
		}

		if (status == WorkerStatus::DONE)
		{
			ptr->onWorkDone(workerId, taskCode, resultBundle);
		}
		else
		{
			ptr->onWorkError(workerId, taskCode);
		}

		data = nullptr;
		callback = std::weak_ptr<INotifyOnCompletion>();
		status = WorkerStatus::CLOSED;
		resultBundle = nullptr;
	}
}