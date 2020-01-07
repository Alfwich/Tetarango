#include "Line.h"

#include "ui/physic/body/Body.h"

namespace
{
	const auto bodyId = "line-body";
	const auto dynamicPropName = "line-dynamic";
}

namespace AW
{
	Line::Line()
	{
		layoutSpace = AW::LayoutSpace::World;
		setScreenSize(100, 1);
		GORegister(Line);
	}

	void Line::onCreateChildren()
	{
		// Will be used as a composite body
		if (std::dynamic_pointer_cast<AW::Body>(parent.lock()) != nullptr) return;

		const auto body = std::make_shared<AW::Body>();
		body->name = bodyId;
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

	void Line::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>(bodyId);
	}

	std::shared_ptr<AW::Renderable> Line::getRenderableBody()
	{
		return std::dynamic_pointer_cast<Renderable>(shared_from_this());
	}

	std::shared_ptr<AW::Body> Line::getBodyObject()
	{
		return body;
	}

	const AW::BodyType Line::getBodyType() const
	{
		return AW::BodyType::Line;
	}

	void Line::setDynamic(bool flag)
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

	bool Line::getDynamic()
	{
		return serializationClient->getBool(dynamicPropName, true);
	}

}