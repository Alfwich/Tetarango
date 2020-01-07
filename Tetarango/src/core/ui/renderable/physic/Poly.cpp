#include "Poly.h"

namespace
{
	const auto dynamicPropName = "poly-d";
}

namespace AW
{
	Poly::Poly()
	{
		layoutSpace = AW::LayoutSpace::World;
		GORegister(Poly);
	}

	void Poly::onCreateChildren()
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

	std::shared_ptr<AW::Body> Poly::getBodyObject()
	{
		return body;
	}

	const AW::BodyType Poly::getBodyType() const
	{
		return AW::BodyType::Polygon;
	}

}