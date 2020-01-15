#include "Network.h"

#include "httplib.h"

namespace
{
	const auto httpProtocolPrefix = "http://";
}

namespace AW
{
	void Network::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	void Network::processHostAndPath(std::string& host, std::string& path)
	{
		if (!host.empty() && host.rfind(httpProtocolPrefix) == 0) host = host.substr(strlen(httpProtocolPrefix));
		if (!path.empty() && path[0] != '/') path = '/' + path;
	}

	std::tuple<std::string, std::string> Network::breakUrlIntoHostAndPath(const std::string& url)
	{
		const std::string protocol = url.rfind(httpProtocolPrefix) == 0 ? httpProtocolPrefix : "";
		const auto hostname = url.substr(protocol.size(), url.find_first_of("/", protocol.size()) - protocol.size());
		const auto path = url.substr(protocol.size() + hostname.size());

		return std::make_tuple(protocol + hostname, path);
	}

	int Network::startRequest(const std::shared_ptr<NetworkRequestBundle>& bundle)
	{
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

		Logger::instance()->log("Network::Started network request for host: " + bundle->host + ", path: " + bundle->path + ", on worker: " + std::to_string(id));

		return id;
	}

	int Network::createBundleAndStartRequest(NetworkRequestMethod method, std::string host, std::string path, std::weak_ptr<INetworkListener> listener)
	{
		processHostAndPath(host, path);

		const auto bundle = std::make_shared<NetworkRequestBundle>(NetworkRequestMethod::Get, host, path, listener);
		const auto id = startRequest(bundle);

		requestMap[id] = bundle;

		return id;
	}

	void Network::endRequest(const WORKER_ID id)
	{
		if (requestMap.count(id) == 0)
		{
			Logger::instance()->logCritical("Network::Attempted to end request without associated worker");
			return;
		}

		const auto bundle = requestMap[id];
		requestMap.erase(id);

		Logger::instance()->log("Network::Ended network request for worker: " + std::to_string(id) + ", status: " + std::to_string(bundle->status));

		const auto listener = bundle->listener.lock();
		if (listener != nullptr)
		{
			listener->onNetworkCallCompleted(bundle->status, bundle->body);
		}
	}

	int Network::httpGet(std::string host, std::string path, std::weak_ptr<INetworkListener> listener)
	{
		return createBundleAndStartRequest(NetworkRequestMethod::Get, host, path, listener);
	}

	int Network::httpGet(std::string url, std::weak_ptr<INetworkListener> listener)
	{
		const auto hostAndPath = breakUrlIntoHostAndPath(url);
		return httpGet(std::get<0>(hostAndPath), std::get<1>(hostAndPath), listener);
	}

	void Network::onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result)
	{
		endRequest(workerId);
	}
}
