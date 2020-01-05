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
		GORegister(NineSlice);
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
			fragmentShader->setFloatIUParam("targetWidth", getScreenWidth());
			fragmentShader->setFloatIUParam("targetHeight", getScreenHeight());

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
			fragmentShader->setFloatIUParam("targetWidth", getScreenWidth());
			fragmentShader->setFloatIUParam("targetHeight", getScreenHeight());
		}
	}

	void NineSlice::setScreenWidth(double width)
	{
		Animated::setScreenWidth(width);
		layout();
	}

	void NineSlice::setScreenHeight(double height)
	{
		Animated::setScreenHeight(height);
		layout();
	}

	void NineSlice::setCurrentAnimation(std::string animationName)
	{
		Animated::setCurrentAnimation(animationName);

		const auto clipRect = getTextureClipRect();
		if (fragmentShader != nullptr && clipRect != nullptr)
		{
			fragmentShader->setFloatIUParam("targetWidth", getScreenWidth());
			fragmentShader->setFloatIUParam("targetHeight", getScreenHeight());
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