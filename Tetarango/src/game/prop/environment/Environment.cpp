#include "Environment.h"

namespace
{
	const std::vector<AW::Color> standardColors = {
		AW::Color(0x005086ff), // Early Light
		AW::Color(0xf69649ff), // Dawn
		AW::Color(0xf69649ff), // Dawn
		AW::Color(0x65bbe3ff), // PrePost-Day
		AW::Color(0x65bbe3ff), // PrePost-Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x73c8f1ff), // Day
		AW::Color(0x65bbe3ff), // PrePost-Day
		AW::Color(0x65bbe3ff), // PrePost-Day
		AW::Color(0xea4b00ff), // Dusk
		AW::Color(0xea4b00ff), // Dusk
		AW::Color(0x1b1722ff), // PrePost-Night
		AW::Color(0x1b1722ff), // PrePost-Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x0e1425ff), // Night
		AW::Color(0x1b1722ff), // PrePost-Night
		AW::Color(0x1b1722ff)  // PrePost-Night
	};

	const auto dayLengthInSecondsParamKey = "env-length";
	const auto currentTimeParamKey = "env-ct";
	const auto parallaxAmountXParamKey = "env-p-amt-x";
	const auto parallaxAmountYParamKey = "env-p-amt-y";

	const auto noiseTextureName = "noise-solid-512";
	const auto mtn1TextureName = "env-mtn-1";

	const auto layoutUpdateThreshold = 32;
	const auto bodyHOffset = 150.0;
	const auto bodyVOffset = 50.0;
}

namespace AWGame
{
	Environment::Environment()
	{
		GORegister(Environment);
		renderTextureMode = AW::RenderTextureMode::LinearWrapping;
	}

	void Environment::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/noise/noise-solid-512.png", noiseTextureName);
		modules->texture->loadTexture("res/image/prop/environment/mtn1.png", mtn1TextureName);
	}

	void Environment::updateBackgroundGradient()
	{
		const auto currentGameTimeSeconds = currentGameTime / 1000.0;
		const auto p = ((currentGameTime / 1000.0) - std::floor(currentGameTimeSeconds)) / 1.0;
		const auto colorA = standardColors[(int)std::floor((int)(currentGameTimeSeconds) % standardColors.size())];
		const auto colorB = standardColors[(int)std::floor(((int)(currentGameTimeSeconds)+1) % standardColors.size())];
		const auto colorC = standardColors[(int)std::floor(((int)(currentGameTimeSeconds)+2) % standardColors.size())];

		environmentColor = colorA.lerp(colorB, p);
		const auto cAN = environmentColor.asNormalized();
		const auto cB = colorB.lerp(colorC, p).asNormalized();

		fragmentShader->setFloatV4IUParam("fColorA", cAN.r, cAN.g, cAN.b, cAN.a);
		fragmentShader->setFloatV4IUParam("fColorB", cB.r, cB.g, cB.b, cB.a);

		environmentColor.r = AW::NumberHelper::clamp(environmentColor.r * 2.0 + 50.0, 0.0, 255.0);
		environmentColor.g = AW::NumberHelper::clamp(environmentColor.g * 2.0 + 50.0, 0.0, 255.0);
		environmentColor.b = AW::NumberHelper::clamp(environmentColor.b * 2.0 + 50.0, 0.0, 255.0);
	}

	void Environment::updateParallaxContainer()
	{
		parallaxContainer1->setScreenPosition(getScreenHalfWidth() + parallaxAmountX * 0.25, getScreenHalfHeight() + parallaxAmountY * 0.25);
		parallaxContainer2->setScreenPosition(getScreenHalfWidth() + parallaxAmountX * 0.5, getScreenHalfHeight() + parallaxAmountY * 0.5);
		farBackground->topLeftAlignSelf(0.0, 1000.0 + parallaxAmountY * 0.01);
	}

	void Environment::updateBodies()
	{
		const auto currentGameTimeSeconds = currentGameTime / 1000.0;
		const auto cycleP = (currentGameTimeSeconds / standardColors.size()) - std::floor(currentGameTimeSeconds / standardColors.size());

		const auto cycleOffset = 0.0;
		const auto moonR = (cycleP + 0.0 + cycleOffset) * AW::NumberHelper::PI * 2.0;
		const auto sunR = (cycleP + 0.5 + cycleOffset) * AW::NumberHelper::PI * 2.0;

		sun->setScreenPosition(
			std::cos(sunR) * (getScreenHalfWidth() - bodyHOffset) + getScreenHalfWidth(),
			std::sin(sunR) * (getScreenHalfHeight() - bodyVOffset) + getScreenHalfHeight()
		);

		moon->setScreenPosition(
			std::cos(moonR) * (getScreenHalfWidth() - bodyHOffset) + getScreenHalfWidth(),
			std::sin(moonR) * (getScreenHalfHeight() - bodyVOffset) + getScreenHalfHeight()
		);
	}

	void Environment::updateGameTime(const double& frameTime)
	{
		currentGameTime += (frameTime / dayLengthInSeconds) * standardColors.size();
	}

	void Environment::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-color", "f-vgradient", "f-repeat", "f-dither-texture" });
		fragmentShader->setFloatIUParam("fRepeat", 64.0);
		fragmentShader->setFloatIUParam("fDitherTextureAmount", 0.015);
	}

	void Environment::onInitialAttach()
	{
		setTexture(noiseTextureName);
		enableEnterFrame();

		layoutUpdateTimer = modules->time->createTimer(AW::TimeScope::Game);
		layoutUpdateTimer->start();
	}

	void Environment::onCreateChildren()
	{
		sun = std::make_shared<AW::Circle>();
		sun->renderColorMode = AW::RenderColorMode::Absolute;
		sun->name = "sun";
		sun->setColor(252, 212, 64);
		sun->setScreenSize(150.0, 150.0);
		sun->setEdgeFadeDistance(1.75);
		add(sun);

		moon = std::make_shared<AW::Circle>();
		moon->renderColorMode = AW::RenderColorMode::Absolute;
		moon->name = "moon";
		moon->setColor(225, 225, 255);
		moon->setScreenSize(150.0, 150.0);
		moon->setEdgeFadeDistance(0.15);
		add(moon);

		farBackground = std::make_shared<AW::Rectangle>();
		farBackground->name = "fb";
		farBackground->setScreenSize(4000, 1000);
		farBackground->setColor(48, 155, 10);
		farBackground->topLeftAlignSelf(0.0, 1000.0);
		add(farBackground);

		parallaxContainer1 = std::make_shared<AW::Container>();
		parallaxContainer1->name = "pc1";
		parallaxContainer1->setColor(AW::Color(192, 192, 192));
		add(parallaxContainer1);

		for (auto x = -5; x < 5; ++x)
		{
			const auto mtn = std::make_shared<AW::Element>();
			const auto mtnSize = 1024;
			mtn->setTexture(mtn1TextureName);
			mtn->setScreenSize(mtnSize * 2.0, mtnSize);
			mtn->setScreenPosition(x * 600.0 + AW::NumberHelper::random(-100, 100), AW::NumberHelper::random(-100, 100.0));
			mtn->setColor(92, 192, 20);
			parallaxContainer1->add(mtn);
		}

		parallaxContainer2 = std::make_shared<AW::Container>();
		parallaxContainer2->name = "pc2";
		add(parallaxContainer2);

		for (auto x = -5; x < 5; ++x)
		{
			const auto mtn = std::make_shared<AW::Element>();
			const auto mtnSize = 800;
			mtn->setTexture(mtn1TextureName);
			mtn->setScreenSize(mtnSize, mtnSize);
			mtn->setScreenPosition(x * 600.0 + AW::NumberHelper::random(-100, 100), AW::NumberHelper::random(100.0, 200.0));
			mtn->setColor(92, 192, 20);
			parallaxContainer2->add(mtn);
		}
	}

	void Environment::onLayoutChildren()
	{
		updateParallaxContainer();
		updateBackgroundGradient();
		updateBodies();
	}

	void Environment::onChildrenHydrated()
	{
		sun = findChildWithName<AW::Circle>("sun");
		moon = findChildWithName<AW::Circle>("moon");
		parallaxContainer1 = findChildWithName<AW::Container>("pc1");
		parallaxContainer2 = findChildWithName<AW::Container>("pc2");
		farBackground = findChildWithName<AW::Rectangle>("fb");
	}

	void Environment::onEnterFrame(const double& frameTime)
	{
		updateGameTime(frameTime);
		if (layoutUpdateTimer->isAboveThresholdAndRestart(layoutUpdateThreshold))
		{
			layout();
		}
	}

	void Environment::setLengthOfDayInSeconds(double length)
	{
		dayLengthInSeconds = AW::NumberHelper::clamp(length, 1.0, DBL_MAX);
	}

	void Environment::setParallaxAmount(double amountX, double amountY)
	{
		parallaxAmountX = amountX;
		parallaxAmountY = amountY;
		updateParallaxContainer();
	}

	const AW::Color& Environment::getEnvironmentColor()
	{
		return environmentColor;
	}

	std::shared_ptr<AW::SerializationClient> Environment::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__env__", hint);

		currentGameTime = client->serializeDouble(currentTimeParamKey, currentGameTime);
		dayLengthInSeconds = client->serializeDouble(dayLengthInSecondsParamKey, dayLengthInSeconds);

		parallaxAmountX = client->serializeDouble(parallaxAmountXParamKey, parallaxAmountX);
		parallaxAmountY = client->serializeDouble(parallaxAmountYParamKey, parallaxAmountY);

		return Element::doSerialize(hint);
	}
}