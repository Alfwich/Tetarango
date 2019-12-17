#pragma once

#include <memory>
#include "SDL.h"
#include "SDL_thread.h"
#include "engine/module/logger/Logger.h"
#include "INotifyOnCompletion.h"
#include "WorkerStatus.h"
#include "AsyncBundles.h"

namespace AW
{

	class Worker : public std::enable_shared_from_this<Worker>
	{

	protected:
		Worker() {};

		std::weak_ptr<INotifyOnCompletion> callback;
		void* (*toDo)(void*data);
		void* data;
		void* result;
		std::shared_ptr<AsyncResultBundle> resultBundle;

	public:
		Worker(void* data, void* (*toDo)(void*data), std::weak_ptr<INotifyOnCompletion> callback, WORKER_ID workerId);

		WORKER_ID workerId = 0;
		WorkerStatus status;
		WorkerTaskCode taskCode;
		WORKER_ID getId();

		virtual void doWork();
		void* getResult();
		void doCallback();
	};



	template <typename T, typename S>
	class SharedPtrWorker : public Worker
	{
		std::shared_ptr<S> dataReference;
		std::shared_ptr<T> resultReference;
		std::shared_ptr<T>(*toDo)(std::shared_ptr<S> data);
	public:
		SharedPtrWorker(std::shared_ptr<S> data, std::shared_ptr<T>(*toDo)(std::shared_ptr<S> data), std::weak_ptr<INotifyOnCompletion> callback, WORKER_ID workerId, WorkerTaskCode code);
		void doWork();
	};

	template<typename T, typename S>
	inline SharedPtrWorker<T, S>::SharedPtrWorker(std::shared_ptr<S> data, std::shared_ptr<T>(*toDo)(std::shared_ptr<S> data), std::weak_ptr<INotifyOnCompletion> callback, WORKER_ID workerId, WorkerTaskCode code)
	{
		dataReference = data;
		this->workerId = workerId;
		status = WorkerStatus::NOT_STARTED;
		taskCode = code;
		this->toDo = toDo;
		this->callback = callback;
	}

	template<typename T, typename S>
	inline void SharedPtrWorker<T, S>::doWork()
	{
		const auto resultType = std::string(typeid(T).name());
		resultReference = toDo(dataReference);

		if (resultType == "void")
		{
			status = WorkerStatus::DONE;
			result = nullptr;
		}
		else if (resultReference != nullptr)
		{
			status = WorkerStatus::DONE;
			result = &resultReference;
		}
		else
		{
			status = WorkerStatus::ERROR;
			result = nullptr;
		}

		resultBundle = std::make_shared<AsyncResultBundle>(status, taskCode, result);
	}
}
