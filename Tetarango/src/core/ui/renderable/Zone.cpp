#include "Zone.h"

#include "ui/renderable/primitive/Rectangle.h"

namespace AW
{

	Zone::Zone()
	{
		zIndex = 20;
		setRequestReconciliation(true);
		markIsZone();
		registerSerialization<Zone>();
	}

	void Zone::visualize()
	{
		serializationClient->setBool("visualize", true);
		const auto rec = std::make_shared<Rectangle>();
		rec->setSize(getWidth(), getHeight());
		rec->setPosition(getWidth() / 2.0, getHeight() / 2.0);
		rec->setColor(0xff, 0, 0, 128);
		add(rec);
	}

	void Zone::setRequestReconciliation(bool flag)
	{
		this->requestReconciliation = flag;
		serializationClient->setBool("rqR", true);
	}

	void Zone::onInitialAttach()
	{
		Container::onInitialAttach();

		if (modules->gameConfig->getConfigBool(Config::Param::visualizeContainers) && !serializationClient->getBool("visualize"))
		{
			visualize();
		}

		requestReconciliation = serializationClient->getBool("rqR");
	}

	bool Zone::onCollision(std::shared_ptr<ICollidable> you, int yourScope, std::weak_ptr<ICollidable> other, int otherScope)
	{
		const auto callback = notifyOnCollision.lock();

		if (callback != nullptr)
		{
			return callback->onCollision(you, yourScope, other, otherScope);
		}

		return requestReconciliation;
	}

	bool Zone::onReconcileCollision(std::shared_ptr<ICollidable> you, std::shared_ptr<ICollidable> other)
	{
		const auto callback = notifyOnReconcileCollision.lock();

		if (callback != nullptr)
		{
			return callback->onReconcileCollision(you, other);
		}

		return false;
	}
}
