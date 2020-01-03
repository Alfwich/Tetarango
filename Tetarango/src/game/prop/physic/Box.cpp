#include "Box.h"

namespace
{
	const auto dynamicPropName = "awg-box-d";
}

namespace AWGame
{
	Box::Box()
	{
		setSize(32, 32);
		registerGameObject<Box>(__FUNCTION__);
	}

	void Box::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-color" });
	}

	void Box::onCreateChildren()
	{
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

	void Box::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>("body");
	}

	void Box::setDynamic(bool flag)
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

	bool Box::getDynamic()
	{
		return serializationClient->getBool(dynamicPropName, true);
	}

	std::shared_ptr<AW::Renderable> Box::getRenderableBody()
	{
		return std::dynamic_pointer_cast<Renderable>(shared_from_this());
	}

	std::shared_ptr<AW::Body> Box::getBodyObject()
	{
		return body;
	}
}
