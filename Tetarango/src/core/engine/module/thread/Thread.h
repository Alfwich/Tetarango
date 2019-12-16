#pragma once

#include <iostream>
#include <list> 
#include <unordered_map>
#include <queue>
#include "SDL.h"
#include "SDL_thread.h"
#include "engine/module/IBaseModule.h"
#include "INotifyOnCompletion.h"

namespace AWCore
{
	class AsyncResultBundle;
	class Worker;

	template<typename T, typename S>
	class SharedPtrWorker;

	class Thread : public IBaseModule
	{
		WORKER_ID nextWorkerId = 0;
		SDL_threadID mainThreadId;
		SDL_sem* workQueueLock;
		SDL_sem* resultQueueLock;
		std::list<SDL_Thread*> threads;

		std::queue<std::shared_ptr<Worker>> resultQueue;
		std::unordered_map<WORKER_ID, bool> outstandingWorkers;
		std::queue<std::shared_ptr<Worker>> workQueue;
		bool running = true;

		WORKER_ID startWorker(std::shared_ptr<Worker> worker);

	public:
		Thread();
		virtual ~Thread();

		void processWorkers();
		WORKER_ID doWork(void* data, void* (*toDo)(void*data), std::weak_ptr<INotifyOnCompletion> callback);

		template <typename T, typename S>
		WORKER_ID doWorkSharedPtr(std::shared_ptr<S> data, std::shared_ptr<T>(*toDo)(std::shared_ptr<S> data), std::weak_ptr<INotifyOnCompletion> callback, WorkerTaskCode code = WorkerTaskCode::UNSPECIFIED);

		bool isMainThread();
		bool applicationIsRunning();
		bool workQueueIsEmpty();

		std::shared_ptr<Worker> getWorkerFromWorkQueue();
		void putWorkerIntoResultQueue(std::shared_ptr<Worker> resultBundle);

		void onInit();
		void onCleanup();
	};

	template<typename T, typename S>
	inline WORKER_ID Thread::doWorkSharedPtr(std::shared_ptr<S> dataPtr, std::shared_ptr<T>(*toDo)(std::shared_ptr<S>data), std::weak_ptr<INotifyOnCompletion> callback, WorkerTaskCode code)
	{
		if (SDL_GetThreadID(NULL) != this->mainThreadId)
		{
			Logger::instance()->log("Thread::Failed to create worker, the calling thread is not the main thread.");
			return -1;
		}

		const auto worker = std::make_shared<SharedPtrWorker<T, S>>(dataPtr, toDo, callback, this->nextWorkerId++, code);
		return this->startWorker(worker);
	}
}

#include "Worker.h"
