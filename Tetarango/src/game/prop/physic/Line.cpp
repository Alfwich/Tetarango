#include "Line.h"

#include "ui/physic/body/Body.h"

namespace
{
	const auto bodyId = "line-body";
	const auto dynamicPropName = "line-dynamic";
}

namespace AWGame
{
	Line::Line()
	{
		setSize(100, 1);
		GORegister(Line);
	}

	void Line::onCreateChildren()
	{
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
		body->setBodyType(AW::BodyType::Line);
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