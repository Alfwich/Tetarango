#include "Thread.h"

namespace
{
	const auto workerThreadName = "AW_Worker_";
}

namespace AW
{
	Thread::Thread()
	{
		mainThreadId = SDL_GetThreadID(NULL);
		resultQueueLock = SDL_CreateSemaphore(1);
		workQueueLock = SDL_CreateSemaphore(1);
	}

	Thread::~Thread()
	{
		SDL_DestroySemaphore(resultQueueLock);
		SDL_DestroySemaphore(workQueueLock);
	}

	void Thread::processWorkers()
	{
		if (!isMainThread())
		{
			Logger::instance()->log("Thread::Failed to processWorkers, the calling thread is not the main thread");
			return;
		}

		// Do work on main thread if we don't have worker threads
		if (threads.size() == 0)
		{
			while (!workQueue.empty())
			{
				const auto worker = getWorkerFromWorkQueue();
				worker->doWork();
				putWorkerIntoResultQueue(worker);
			}
		}

		SDL_SemWait(resultQueueLock);

		while (!resultQueue.empty())
		{
			const auto worker = resultQueue.front();
			const auto workerId = worker->getId();
			switch (worker->status)
			{
			case WorkerStatus::ERROR:
				Logger::instance()->logCritical("Thread::Worker error for id=" + std::to_string(workerId));
			case WorkerStatus::DONE:
				Logger::instance()->log("Thread::Worker finished for id=" + std::to_string(workerId));
				worker->doCallback();
				break;
			}
			resultQueue.pop();
		}

		SDL_SemPost(resultQueueLock);
	}

	WORKER_ID Thread::doWork(void* data, void* (*toDo)(void* data), std::weak_ptr<INotifyOnCompletion> callback)
	{
		if (!isMainThread())
		{
			Logger::instance()->log("Thread::Failed to create worker, the calling thread is not the main thread");
			return -1;
		}

		const auto worker = std::make_shared<Worker>(data, toDo, callback, nextWorkerId++);
		return startWorker(worker);
	}

	WORKER_ID Thread::startWorker(std::shared_ptr<Worker> worker)
	{
		const auto workerId = worker->getId();
		Logger::instance()->log("Thread::Worker created with for id=" + std::to_string(worker->getId()) + ", added to the work queue");

		SDL_SemWait(workQueueLock);
		outstandingWorkers[worker->getId()] = true;
		workQueue.push(worker);
		SDL_SemPost(workQueueLock);

		return workerId;
	}

	bool Thread::isMainThread()
	{
		return SDL_GetThreadID(NULL) == mainThreadId;
	}

	std::shared_ptr<Worker> Thread::getWorkerFromWorkQueue()
	{
		SDL_SemWait(workQueueLock);

		auto result = std::shared_ptr<Worker>();

		if (workQueue.size() != 0)
		{
			result = workQueue.front();
			workQueue.pop();
		}

		SDL_SemPost(workQueueLock);

		return result;
	}

	void Thread::putWorkerIntoResultQueue(std::shared_ptr<Worker> worker)
	{
		SDL_SemWait(resultQueueLock);
		resultQueue.push(worker);
		outstandingWorkers.erase(worker->getId());
		SDL_SemPost(resultQueueLock);
	}

	bool Thread::applicationIsRunning()
	{
		return running;
	}

	bool Thread::workQueueIsEmpty()
	{
		return workQueue.empty();
	}

	bool Thread::hasExtraWorkerThreads()
	{
		return threads.size() > 1;
	}

	bool Thread::hasWorkerThreads()
	{
		return !threads.empty();
	}

	void Thread::onInit()
	{
		if (!isMainThread())
		{
			Logger::instance()->log("Thread::Failed to onInit, the calling thread is not the main thread");
			return;
		}

		if (!threads.empty())
		{
			Logger::instance()->log("Thread::Failed to onInit, we already have worker threads in operation");
			return;
		}

		Logger::instance()->log("Thread::Creating worker threads");
		const auto numWorkerThreads = NumberHelper::clamp(SDL_GetCPUCount() * 2, 1, 8);
		for (int i = 0; i < numWorkerThreads; ++i)
		{
			const auto thread = SDL_CreateThread([](void* data) -> int
				{
					const auto thread = (Thread*)data;
					std::shared_ptr<Worker> worker = nullptr;

					while (thread->applicationIsRunning())
					{
						worker = thread->getWorkerFromWorkQueue();

						if (worker != nullptr)
						{
							worker->doWork();
							thread->putWorkerIntoResultQueue(worker);
						}
						else
						{
							SDL_Delay(100);
						}
					}

					return 0;
				}, std::string(workerThreadName + std::to_string(i)).c_str(), this);

			threads.push_back(thread);
		}
	}

	void Thread::onCleanup()
	{
		if (!isMainThread())
		{
			Logger::instance()->logCritical("Thread::Failed to onCleanup, the calling thread is not the main thread");
			return;
		}

		Logger::instance()->log("Thread::Draining work/result queue");
		while (!workQueue.empty() || !resultQueue.empty() || !outstandingWorkers.empty())
		{
			this->processWorkers();
			SDL_Delay(100);
		}

		running = false;
		Logger::instance()->log("Thread::Waiting for worker threads to finish");
		for (const auto thread : threads)
		{
			auto threadId = SDL_GetThreadID(thread);
			SDL_WaitThread(thread, NULL);
		}

		threads.clear();
	}
}
