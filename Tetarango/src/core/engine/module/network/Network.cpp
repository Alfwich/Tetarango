#include "Network.h"

#include "httplib.h"

namespace AW
{
	void Network::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	int Network::httpGet(std::string host, std::string path, std::weak_ptr<INetworkListener> listener)
	{
		const auto bundle = std::make_shared<NetworkRequestBundle>(host, path, listener);
		const auto id = thread->doWork(bundle.get(), [](void* data)
			{
				NetworkRequestBundle* bundle = (NetworkRequestBundle*)data;
				httplib::Client client(bundle->host.c_str());
				auto response = client.Get(bundle->path.c_str());
				if (response)
				{
					bundle->status = response->status;
					bundle->body = response->body;
				}

				return data;
			}, weak_from_this());

		requestMap[id] = bundle;

		return id;
	}

	void Network::onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result)
	{
		if (requestMap.count(workerId) == 0)
		{
			Logger::instance()->logCritical("Network::Got threading callback without associated worker listing");
			return;
		}

		const auto bundle = requestMap[workerId];
		requestMap.erase(workerId);

		const auto listener = bundle->listener.lock();
		if (listener != nullptr)
		{
			listener->onNetworkCallCompleted(bundle->status, bundle->body);
		}
	}
}
