#pragma once

#include "engine/module/IBaseModule.h"

#include "engine/module/thread/Thread.h"
#include "INetworkListener.h"

namespace AW
{
	enum class NetworkRequestMethod
	{
		Get,
		Post
	};

	class NetworkRequestBundle;

	class Network : public IBaseModule
	{
		friend class NetworkRequestBundle;
		std::shared_ptr<Thread> thread;

		int requestId = 0;
		std::unordered_map<WORKER_ID, std::shared_ptr<NetworkRequestBundle>> requestMap;

		void processHostAndPath(std::string& host, std::string& path);
		std::tuple<std::string, std::string> breakUrlIntoHostAndPath(const std::string& url);

		int startRequest(const std::shared_ptr<NetworkRequestBundle>& bundle);
		int createBundleAndStartRequest(NetworkRequestMethod method, std::string host, std::string path, std::weak_ptr<INetworkListener> listener);
		int createBundleWithParamsAndStartRequest(NetworkRequestMethod method, std::string host, std::string path, const std::unordered_map<std::string, std::string>& params, std::weak_ptr<INetworkListener> listener);
		void endRequest(const WORKER_ID id);

		void doRequest(NetworkRequestBundle* bundle);

	public:
		void bindThread(std::shared_ptr<Thread> thread);

		int httpGet(std::string url, std::weak_ptr<INetworkListener> listener);
		int httpPost(std::string url, const std::unordered_map<std::string, std::string>& params, std::weak_ptr<INetworkListener> listener);

		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result) override;
	};

	class NetworkRequestBundle
	{
		friend class Network;
		Network* svc;

		void doRequest() { svc->doRequest(this); }

	public:
		NetworkRequestBundle(Network* svc, NetworkRequestMethod method, std::string host, std::string path, const std::unordered_map<std::string, std::string>& params, std::weak_ptr<INetworkListener> listener)
			: svc(svc), method(method), host(host), path(path), listener(listener), params(params)
		{
		}

		const NetworkRequestMethod method;
		const std::string host, path;
		const std::weak_ptr<INetworkListener> listener;

		const std::unordered_map<std::string, std::string> params;

		int status = 0;
		std::string body;
	};

}
