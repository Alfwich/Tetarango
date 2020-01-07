#include "Chain.h"

namespace
{
	const auto dynamicPropName = "poly-d";
}

namespace AWGame
{
	Chain::Chain()
	{
		layoutSpace = AW::LayoutSpace::World;
		setFilled(false);
		GORegister(Chain);
	}

	void Chain::onCreateChildren()
	{
		// Will be used as a composite body
		if (std::dynamic_pointer_cast<AW::Body>(parent.lock()) != nullptr) return;

		body = std::make_shared<AW::Body>();
		body->name = "body";
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

	const AW::BodyType Chain::getBodyType() const
	{
		return AW::BodyType::Chain;
	}

}