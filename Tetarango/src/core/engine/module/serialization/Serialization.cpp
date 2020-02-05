#include "Serialization.h"

namespace AW
{
	Serialization::Serialization()
	{
	}

	void Serialization::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	std::string Serialization::serialize(std::shared_ptr<ISerializable> ser, bool humanReadable)
	{
		Serializer serializer(humanReadable);
		return serializer.serialize(ser);
	}

	WORKER_ID Serialization::serializeAsync(std::shared_ptr<ISerializable> ser, std::weak_ptr<INotifyOnCompletion> callback)
	{
		auto thisServicePtr = std::static_pointer_cast<Serialization>(shared_from_this());
		auto bundle = std::make_shared<AsyncOperationBundle<Serialization, ISerializable>>(thisServicePtr, ser);

		return thread->doWorkSharedPtr<std::string, AsyncOperationBundle<Serialization, ISerializable>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<Serialization, ISerializable>> bundle) -> std::shared_ptr<std::string>
			{
				try
				{
					const auto ser = bundle->data;
					const auto service = bundle->service;
					const auto result = service->serialize(ser);
					return std::make_shared<std::string>(result);
				}
				catch (...)
				{
					return nullptr;
				}
			}
		, callback, WorkerTaskCode::SERIALIZATION);
	}

	std::shared_ptr<ISerializable> Serialization::hydrate(const std::string& data)
	{
		return Hydrater(data).hydrateData();
	}

	WORKER_ID Serialization::hydrateAsync(const std::string& data, std::weak_ptr<INotifyOnCompletion> callback)
	{
		auto dataPtr = std::make_shared<std::string>(data);
		auto thisServicePtr = std::static_pointer_cast<Serialization>(shared_from_this());
		auto bundle = std::make_shared<AsyncOperationBundle<Serialization, std::string>>(thisServicePtr, dataPtr);

		return thread->doWorkSharedPtr<ISerializable, AsyncOperationBundle<Serialization, std::string>>(bundle,
			[](std::shared_ptr<AsyncOperationBundle<Serialization, std::string>> bundle) -> std::shared_ptr<ISerializable>
			{
				try
				{
					const auto string = bundle->data;
					const auto service = bundle->service;
					return service->hydrate(*string);
				}
				catch (...)
				{
					return nullptr;
				}
			}
		, callback, WorkerTaskCode::HYDRATION);
	}

	bool Serialization::hasSchematic(const std::string& name)
	{
		return Hydrater::hasSchematic(name);
	}

	void Serialization::registerSchematic(std::shared_ptr<Schematic> schematic)
	{
		Hydrater::registerSchematic(schematic);
	}

	std::shared_ptr<Schematic> Serialization::getSchematic(const std::string& name)
	{
		return Hydrater::getSchematic(name);
	}
}
