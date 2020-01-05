#include "Chain.h"

namespace
{
	const auto dynamicPropName = "poly-d";
}

namespace AWGame
{
	Chain::Chain()
	{
		setFilled(false);
		GORegister(Chain);
	}

	void Chain::onCreateChildren()
	{
		body = std::make_shared<AW::Body>();
		body->name = "body";
		body->setBodyType(AW::BodyType::Chain);
		if (getDynamic())
		{
			body->setDynamicBody();
		}
		else
		{
			body->setStaticBody();
		}
		add(body);
	}

	void Chain::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>("body");
	}

	void Chain::setDynamic(bool flag)
	{
		serializationClient->setBool(dynamicPropName, flag);
		if (body != nullptr)
		{
			if (flag)
			{
				body->setDynamicBody();
			}
			else
			{
				body->setStaticBody();
			}
		}
	}

	bool Chain::getDynamic()
	{
		return serializationClient->getBool(dynamicPropName, true);
	}

	std::shared_ptr<AW::Renderable> Chain::getRenderableBody()
	{
		return std::dynamic_pointer_cast<Renderable>(shared_from_this());
	}

	const std::vector<AWVec2<float>> Chain::getBodyWorldPoints()
	{
		return getWorldPoints();
	}

	std::shared_ptr<AW::Body> Chain::getBodyObject()
	{
		return body;
	}

}