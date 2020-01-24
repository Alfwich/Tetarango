#include "BackdropObject.h"

namespace
{
	const auto backdropTypeParamKey = "bdo-b-t";
	const auto mtn1TextureName = "env-mtn-1";
	const auto mtn2TextureName = "env-mtn-2";
	const auto cloudTextureName = "env-cloud";
}

namespace AWGame
{
	BackdropObject::BackdropObject()
	{
		GORegister(BackdropObject);
	}

	void BackdropObject::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/environment/mtn1.png", mtn1TextureName);
		modules->texture->loadTexture("res/image/prop/environment/mtn2.png", mtn2TextureName);
		modules->texture->loadTexture("res/image/prop/environment/cloud.png", cloudTextureName);
	}

	void BackdropObject::onChildrenHydrated()
	{
		setBackdropType((BackdropType)serializationClient->getInt(backdropTypeParamKey));
	}

	void BackdropObject::setBackdropType(BackdropType type)
	{
		serializationClient->setInt(backdropTypeParamKey, (int)type);
		switch (type)
		{
		case AWGame::BackdropType::Mountain1:
			switch (AW::NumberHelper::randomInt(0, 1))
			{
			case 0:
				setTexture(mtn1TextureName);
				break;

			case 1:
				setTexture(mtn2TextureName);
				break;
			}
			break;

		case AWGame::BackdropType::Cloud:
			velocity.x = AW::NumberHelper::random(25.0, 50.0);
			setTexture(cloudTextureName);
			fragmentShader = modules->shader->getShader({ "f-blur", "animated" });
			fragmentShader->setFloatIUParam("fBlurAmount", 0.5);
			fragmentShader->setFloatIUParam("clipX", AW::NumberHelper::randomInt(0, 1) * 256.0);
			fragmentShader->setFloatIUParam("clipY", AW::NumberHelper::randomInt(0, 1) * 256.0);
			fragmentShader->setFloatIUParam("clipWidth", 256.0);
			fragmentShader->setFloatIUParam("clipHeight", 256.0);
			break;

		default:
			break;
		}
	}
}
