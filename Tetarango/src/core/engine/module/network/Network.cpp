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
				bundle->doRequest();
				return data;
			}, weak_from_this());

		Logger::instance()->log("Network::Started network request for host=" + bundle->host + ", with path=" + bundle->path + ", on worker=" + std::to_string(id));

		return id;
	}

	int Network::createBundleAndStartRequest(NetworkRequestMethod method, std::string host, std::string path, std::weak_ptr<INetworkListener> listener)
	{
		processHostAndPath(host, path);

		const auto params = std::unordered_map<std::string, std::string>();
		const auto bundle = std::make_shared<NetworkRequestBundle>(this, method, host, path, params, listener);
		const auto id = startRequest(bundle);

		requestMap[id] = bundle;

		return id;
	}

	int Network::createBundleWithParamsAndStartRequest(NetworkRequestMethod method, std::string host, std::string path, const std::unordered_map<std::string, std::string>& params, std::weak_ptr<INetworkListener> listener)
	{
		processHostAndPath(host, path);

		const auto bundle = std::make_shared<NetworkRequestBundle>(this, method, host, path, params, listener);
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

		Logger::instance()->log("Network::Ended network request for worker=" + std::to_string(id) + ", with status=" + std::to_string(bundle->status));

		const auto listener = bundle->listener.lock();
		if (listener != nullptr)
		{
			listener->onNetworkCallCompleted(bundle->status, bundle->body);
		}
	}

	int Network::httpGet(std::string url, std::weak_ptr<INetworkListener> listener)
	{
		const auto hostAndPath = breakUrlIntoHostAndPath(url);
		return createBundleAndStartRequest(NetworkRequestMethod::Get, std::get<0>(hostAndPath), std::get<1>(hostAndPath), listener);
	}

	int Network::httpPost(std::string url, const std::unordered_map<std::string, std::string>& params, std::weak_ptr<INetworkListener> listener)
	{
		const auto hostAndPath = breakUrlIntoHostAndPath(url);
		return createBundleWithParamsAndStartRequest(NetworkRequestMethod::Post, std::get<0>(hostAndPath), std::get<1>(hostAndPath), params, listener);
	}

	void Network::doRequest(NetworkRequestBundle* bundle)
	{
		httplib::Client client(bundle->host.c_str());

		std::shared_ptr<httplib::Response> response;

		if (bundle->method == NetworkRequestMethod::Get)
		{
			response = client.Get(bundle->path.c_str());
		}
		else if (bundle->method == NetworkRequestMethod::Post)
		{
			httplib::Params params;
			for (auto paramKeyToValue : bundle->params)
			{
				params.emplace(paramKeyToValue.first, paramKeyToValue.second);
			}

			response = client.Post(bundle->path.c_str(), params);
		}

		if (response != nullptr)
		{
			bundle->status = response->status;
			bundle->body = response->body;
		}
	}

	void Network::onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result)
	{
		endRequest(workerId);
	}
}
