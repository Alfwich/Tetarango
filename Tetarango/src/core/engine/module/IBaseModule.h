#pragma once

#include <string>
#include <map>
#include "thread/INotifyOnCompletion.h"
#include "GameConfig.h"

namespace MT
{
	class IBaseModule : public INotifyOnCompletion, public std::enable_shared_from_this<IBaseModule>
	{
	public:
		std::shared_ptr<Config::GameConfig> gameConfig;
		bool isCleanedUp = false;

		virtual void onInit() { /* NO-OP */ };
		virtual void onReady() { /* NO-OP */ };
		virtual void onCleanup() { /* NO-OP */ };
		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode type, std::shared_ptr<AsyncResultBundle> result) { /* NO-OP */ };
		virtual void onWorkError(WORKER_ID workerId, WorkerTaskCode type) { /* NO-OP */ };
	};
}
