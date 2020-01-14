#pragma once

#include "engine/module/IBaseModule.h"

#include "engine/module/thread/Thread.h"
#include "INetworkListener.h"

namespace AW
{
	class NetworkRequestBundle
	{
	public:
		NetworkRequestBundle(std::string host, std::string path, std::weak_ptr<INetworkListener> listener) 
			: host(host), path(path), listener(listener) {}
		const std::string host, path;
		const std::weak_ptr<INetworkListener> listener;

		int status = 0;
		std::string body;
	};

	class Network : public IBaseModule
	{
		std::shared_ptr<Thread> thread;

		std::unordered_map<WORKER_ID, std::shared_ptr<NetworkRequestBundle>> requestMap;

		int requestId = 0;

	public:
		void bindThread(std::shared_ptr<Thread> thread);

		int httpGet(std::string host, std::string path, std::weak_ptr<INetworkListener> listener);

		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result) override;
	};
}
