#include "Poly.h"

namespace
{
	const auto dynamicPropName = "poly-d-";
}

namespace AWGame
{
	Poly::Poly()
	{
		registerGameObject<Poly>(__FUNCTION__);
	}

	void Poly::onCreateChildren()
	{
		body = std::make_shared<AW::Body>();
		body->name = "body";
		body->setBodyType(AW::BodyType::Polygon);
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

	void Poly::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>("body");
	}

	void Poly::setDynamic(bool flag)
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

	bool Poly::getDynamic()
	{
		return serializationClient->getBool(dynamicPropName, true);
	}

	std::shared_ptr<AW::Renderable> Poly::getRenderableBody()
	{
		return std::dynamic_pointer_cast<Renderable>(shared_from_this());
	}

	const std::vector<AWVec2<double>>& Poly::getBodyScreenPoints()
	{
		return getScreenPoints();
	}

}