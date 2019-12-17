#pragma once
#include <string>
#include <vector>
#include <map>
#include "ISerializable.h"
#include "Hydrater.h"
#include "SerializationTags.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/logger/Logger.h"
#include "engine/module/thread/Thread.h"

namespace AW
{

	class Serialization : public IBaseModule
	{
		std::shared_ptr<Thread> thread;

	public:
		Serialization();

		void bindThread(std::shared_ptr<Thread> thread);

		std::string serialize(std::shared_ptr<ISerializable> ser, bool humanReadable = false);
		WORKER_ID serializeAsync(std::shared_ptr<ISerializable> ser, std::weak_ptr<INotifyOnCompletion> callback);

		std::shared_ptr<ISerializable> hydrate(std::string data);
		WORKER_ID hydrateAsync(std::string data, std::weak_ptr<INotifyOnCompletion> callback);

		bool hasSchematic(std::string name);
		void registerSchematic(std::shared_ptr<Schematic> schematic);
		std::shared_ptr<Schematic> getSchematic(std::string name);
	};
}
