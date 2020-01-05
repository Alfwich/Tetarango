#include "Zone.h"

#include "ui/renderable/primitive/Rectangle.h"

namespace AW
{

	Zone::Zone()
	{
		zIndex = 20;
		setRequestReconciliation(true);
		markIsZone();
		GORegister(Zone);
	}

	void Zone::visualize()
	{
		serializationClient->setBool("visualize", true);
		const auto rec = std::make_shared<Rectangle>();
		rec->setScreenSize(getScreenWidth(), getScreenHeight());
		rec->setScreenPosition(getScreenWidth() / 2.0, getScreenHeight() / 2.0);
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
