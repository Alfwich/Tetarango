#include "Zone.h"

#include "ui/renderable/primitive/Rectangle.h"

namespace AW
{

	Zone::Zone()
	{
		zIndex = 20;
		setRequestReconciliation(true);
		markIsZone();
		registerGameObject<Zone>(__FUNCTION__);
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
}
