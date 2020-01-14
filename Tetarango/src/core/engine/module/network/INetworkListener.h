#pragma once


namespace AW
{
	class INetworkListener
	{
	public:
		virtual void onNetworkCallCompleted(int status, std::string& body) { /* NO-OP */ }
		virtual void onNetworkCallCompleted(int id, int status, std::string& body) { onNetworkCallCompleted(status, body); }
	};
}
