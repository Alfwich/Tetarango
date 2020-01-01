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
		registerGameObject<Box>("box");
	}

	void Box::onInitialAttach()
	{
		setTexture("prop-blocks");
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

	void Box::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "block" }, true);

		fragmentShader->setFloatIUParam("clipX", 32.0);
		fragmentShader->setFloatIUParam("clipY", 0.0);
		fragmentShader->setFloatIUParam("clipWidth", 64.0);
		fragmentShader->setFloatIUParam("clipHeight", 64.0);
		fragmentShader->setFloatIUParam("blockBorderSize", 2.0);
		fragmentShader->setFloatIUParam("blockEffect", 0.5);
		fragmentShader->setFloatIUParam("blockEffectP", 0.25);
		fragmentShader->setFloatIUParam("blockEffectG", 0.4);
		fragmentShader->setFloatIUParam("blockCenterFill", AW::NumberHelper::random(0.5, 1.0));
		fragmentShader->setFloatIUParam("fScanlineRetroAmount", 0.25);
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
}
