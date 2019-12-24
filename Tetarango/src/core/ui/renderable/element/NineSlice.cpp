#include "NineSlice.h"
#include "Animated.h"

namespace
{
	const auto cornerSizeParamName = "corner-size";
}

namespace AW
{
	NineSlice::NineSlice()
	{
		sizeToAnimation = false;
		registerGameObject<NineSlice>();
	}

	void NineSlice::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-clip-texture", "f-9slice", "element" });
	}

	void NineSlice::onInitialAttach()
	{
		Animated::onInitialAttach();

		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("cornerSize", getCornerSize());
			fragmentShader->setFloatIUParam("targetWidth", getWidth());
			fragmentShader->setFloatIUParam("targetHeight", getHeight());

			const auto cR = getTextureClipRect();
			if (cR != nullptr)
			{
				fragmentShader->setFloatIUParam("clipX", cR->x);
				fragmentShader->setFloatIUParam("clipY", cR->y);
				fragmentShader->setFloatIUParam("clipWidth", cR->w);
				fragmentShader->setFloatIUParam("clipHeight", cR->h);
			}
			else
			{
				fragmentShader->setFloatIUParam("clipWidth", 0.0);
				fragmentShader->setFloatIUParam("clipHeight", 0.0);
			}
		}
	}

	void NineSlice::onLayoutChildren()
	{
		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("targetWidth", getWidth());
			fragmentShader->setFloatIUParam("targetHeight", getHeight());
		}
	}

	void NineSlice::setWidth(double width)
	{
		Animated::setWidth(width);
		layout();
	}

	void NineSlice::setHeight(double height)
	{
		Animated::setHeight(height);
		layout();
	}

	void NineSlice::setCurrentAnimation(std::string animationName)
	{
		Animated::setCurrentAnimation(animationName);

		const auto clipRect = getTextureClipRect();
		if (fragmentShader != nullptr && clipRect != nullptr)
		{
			fragmentShader->setFloatIUParam("targetWidth", getWidth());
			fragmentShader->setFloatIUParam("targetHeight", getHeight());
		}
	}

	double NineSlice::getCornerSize()
	{
		return serializationClient->getDouble(cornerSizeParamName);
	}

	void NineSlice::setCornerSize(double cornerSize)
	{
		serializationClient->serializeDouble(cornerSizeParamName, cornerSize);

		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("cornerSize", cornerSize);
		}
	}
}