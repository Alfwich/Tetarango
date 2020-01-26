#include "Moon.h"

namespace
{
	const auto moonTextureName = "env-moon-texture";
}

namespace AWGame
{
	Moon::Moon()
	{
		GORegister(Moon);
	}

	void Moon::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/environment/moon.png", moonTextureName);
	}

	void Moon::onInitialAttach()
	{
		renderColorMode = AW::RenderColorMode::Absolute;
		enableEnterFrame();
	}

	void Moon::onCreateChildren()
	{
		moon = std::make_shared<AW::Element>();
		moon->name = "moon";
		moon->setColor(getColor());
		moon->setTexture(moonTextureName);
		add(moon);

		inner = std::make_shared<AW::Element>();
		inner->name = "in";
		inner->setColor(AW::Color::white());
		inner->setAlpha(0.5);
		inner->setTexture(moonTextureName);
		add(inner);

		inner2 = std::make_shared<AW::Element>();
		inner2->name = "in2";
		inner2->setColor(AW::Color::white());
		inner2->setAlpha(0.5);
		inner2->setTexture(moonTextureName);
		add(inner2);

		const auto fs = modules->shader->getShader({ "f-color", "f-circle", "f-repeat", "f-dither-texture" });
		fs->setFloatIUParam("fCircleEdge", 10.0);
		fs->setFloatIUParam("fCircleOffset", 0.1);
		fs->setFloatIUParam("fRepeat", 64.0);
		fs->setFloatIUParam("fDitherTextureAmount", 0.015);

		backgroundEffect = std::make_shared<AW::Element>();
		backgroundEffect->name = "bg-e";
		backgroundEffect->setColor(getColor());
		backgroundEffect->setAlpha(0.15);
		backgroundEffect->setTexture("noise-solid-512");
		backgroundEffect->setFragmentShader(fs);
		backgroundEffect->zIndex = -1;
		add(backgroundEffect);
	}

	void Moon::onChildrenHydrated()
	{
		moon = findChildWithName<AW::Element>("sun");
		inner = findChildWithName<AW::Element>("in");
		inner2 = findChildWithName<AW::Element>("in2");
		backgroundEffect = findChildWithName<AW::Element>("bg-e");
	}

	void Moon::onLayoutChildren()
	{
		moon->setSize(this);
		moon->centerAlignWithin(this);

		inner->setSize(this, -25, -25);
		inner->centerAlignWithin(this);

		inner2->setSize(this, -50, -50);
		inner2->centerAlignWithin(this);

		backgroundEffect->setSize(this, 1500, 1500);
		backgroundEffect->centerAlignWithin(this);
	}

	void Moon::onEnterFrame(const double& frameTime)
	{
		inner->rotateScreen(15.0 * (frameTime / 1000.0));
		inner2->rotateScreen(30.0 * (frameTime / 1000.0));
	}

}
