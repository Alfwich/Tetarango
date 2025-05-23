#include "Box.h"

namespace
{
	const auto dynamicPropName = "awg-box-d";
}

namespace AW
{
	Box::Box()
	{
		layoutSpace = AW::LayoutSpace::World;
		setScreenSize(32, 32);
		GORegister(Box);
	}

	void Box::onBindShaders()
	{
		if (texture != nullptr)
		{
			Element::onBindShaders();
		}
		else if (fragmentShader == nullptr)
		{
			fragmentShader = modules->shader->getShader({ "f-color" });
		}
	}

	void Box::onCreateChildren()
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

	const AW::BodyType Box::getBodyType() const
	{
		return AW::BodyType::Box;
	}

	const AWVec2<float> Box::getBodyWorldSize()
	{
		return AWVec2<float>(getWorldWidth(), getWorldHeight());
	}
}
