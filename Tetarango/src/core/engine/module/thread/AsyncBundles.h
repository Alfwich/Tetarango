#pragma once

#include "WorkerStatus.h"

namespace AW
{
	class AsyncResultBundle
	{
		void* result;

	public:
		AsyncResultBundle(WorkerStatus status, WorkerTaskCode taskCode, void* result)
		{
			this->status = status;
			this->taskCode = taskCode;
			this->result = result;
		};

		WorkerStatus status;
		WorkerTaskCode taskCode;

		void* getRawResult()
		{
			return result;
		}

		template <typename T>
		std::shared_ptr<T> getResult()
		{
			auto ref = static_cast<std::shared_ptr<T>*>(result);
			if (ref != nullptr)
			{
				return *ref;
			}

			return std::shared_ptr<T>();
		}

		template <typename B, typename T>
		std::shared_ptr<T> getResultBaseCast()
		{
			auto ref = static_cast<std::shared_ptr<B>*>(result);
			if (ref != nullptr)
			{
				return std::static_pointer_cast<T>(*ref);
			}

			return std::shared_ptr<T>();
		}
	};

	template <typename T, typename S>
	class AsyncOperationBundle
	{
	public:
		AsyncOperationBundle(std::shared_ptr<T> service, std::shared_ptr<S> data) {
			this->service = std::dynamic_pointer_cast<T>(service);
			this->data = data;
		};
		std::shared_ptr<T> service;
		std::shared_ptr<S> data;
	};


}
