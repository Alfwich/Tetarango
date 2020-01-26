#include "Sun.h"

namespace
{
	const auto sunTextureName = "env-sun-texture";
}

namespace AWGame
{
	Sun::Sun()
	{
		GORegister(Sun);
	}

	void Sun::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/environment/sun.png", sunTextureName);
	}

	void Sun::onInitialAttach()
	{
		renderColorMode = AW::RenderColorMode::Absolute;
		enableEnterFrame();
	}

	void Sun::onCreateChildren()
	{
		sun = std::make_shared<AW::Element>();
		sun->name = "sun";
		sun->setColor(getColor());
		sun->setTexture(sunTextureName);
		add(sun);

		inner = std::make_shared<AW::Element>();
		inner->name = "in";
		inner->setColor(AW::Color::white());
		inner->setAlpha(0.5);
		inner->setTexture(sunTextureName);
		add(inner);

		inner2 = std::make_shared<AW::Element>();
		inner2->name = "in2";
		inner2->setColor(AW::Color::white());
		inner2->setAlpha(0.5);
		inner2->setTexture(sunTextureName);
		add(inner2);

		backgroundEffect = std::make_shared<AW::Circle>();
		backgroundEffect->name = "bg-e";
		backgroundEffect->setColor(getColor());
		backgroundEffect->setEdgeFadeDistance(100.0);
		backgroundEffect->setAlpha(0.5);
		backgroundEffect->zIndex = -1;
		add(backgroundEffect);
	}

	void Sun::onChildrenHydrated()
	{
		sun = findChildWithName<AW::Element>("sun");
		inner = findChildWithName<AW::Element>("in");
		inner2 = findChildWithName<AW::Element>("in2");
		backgroundEffect = findChildWithName<AW::Circle>("bg-e");
	}

	void Sun::onLayoutChildren()
	{
		sun->setSize(this);
		sun->centerAlignWithin(this);

		inner->setSize(this, -25, -25);
		inner->centerAlignWithin(this);

		inner2->setSize(this, -50, -50);
		inner2->centerAlignWithin(this);

		backgroundEffect->setSize(this, 750, 750);
		backgroundEffect->centerAlignWithin(this);
	}

	void Sun::onEnterFrame(const double& frameTime)
	{
		currentTime += frameTime / 1000.0;
		setScale(0.99 + 0.01 * std::sin(currentTime * 2.0 * AW::NumberHelper::PI));
		inner->rotateScreen(25.0 * (frameTime / 1000.0));
		inner2->rotateScreen(50.0 * (frameTime / 1000.0));
	}
}
