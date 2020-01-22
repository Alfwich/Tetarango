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
	const auto parallaxAmountParamKey = "env-p-amt";

	const auto noiseTextureName = "noise-solid-512";

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
	}

	void Environment::updateBackgroundGradient()
	{
		const auto currentGameTimeSeconds = currentGameTime / 1000.0;
		const auto p = ((currentGameTime / 1000.0) - std::floor(currentGameTimeSeconds)) / 1.0;
		const auto colorA = standardColors[(int)std::floor((int)(currentGameTimeSeconds) % standardColors.size())];
		const auto colorB = standardColors[(int)std::floor(((int)(currentGameTimeSeconds)+1) % standardColors.size())];
		const auto colorC = standardColors[(int)std::floor(((int)(currentGameTimeSeconds)+2) % standardColors.size())];

		const auto cA = colorA.lerp(colorB, p).asNormalized();
		const auto cB = colorB.lerp(colorC, p).asNormalized();

		fragmentShader->setFloatV4IUParam("fColorA", cA.r, cA.g, cA.b, cA.a);
		fragmentShader->setFloatV4IUParam("fColorB", cB.r, cB.g, cB.b, cB.a);
	}

	void Environment::updateParallaxContainer()
	{
		parallaxContainer->setScreenPosition(getScreenHalfWidth() + parallaxAmount * 0.5, getScreenHalfHeight());
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
		sun->name = "sun";
		sun->setColor(252, 212, 64);
		sun->setScreenSize(150.0, 150.0);
		sun->setEdgeFadeDistance(0.25);
		add(sun);

		moon = std::make_shared<AW::Circle>();
		moon->name = "moon";
		moon->setColor(225, 225, 255);
		moon->setScreenSize(200.0, 200.0);
		moon->setEdgeFadeDistance(0.15);
		add(moon);

		parallaxContainer = std::make_shared<AW::Container>();
		parallaxContainer->name = "pc";
		add(parallaxContainer);

		for (auto x = 0; x < 10; ++x)
		{
			const auto rect = std::make_shared<AW::Rectangle>();
			const auto mtnSize = AW::NumberHelper::random(800, 1600);
			rect->setScreenSize(mtnSize, mtnSize);
			rect->setScreenPosition(AW::NumberHelper::random(-2000, 2000), 400);
			rect->setColor(AW::Color::random());
			rect->setScreenRotation(45);
			parallaxContainer->add(rect);
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
		parallaxContainer = findChildWithName<AW::Container>("pc");
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

	void Environment::setParallaxAmount(double amount)
	{
		parallaxAmount = amount;
	}

	std::shared_ptr<AW::SerializationClient> Environment::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__env__", hint);

		currentGameTime = client->serializeDouble(currentTimeParamKey, currentGameTime);
		dayLengthInSeconds = client->serializeDouble(dayLengthInSecondsParamKey, dayLengthInSeconds);
		parallaxAmount = client->serializeDouble(parallaxAmountParamKey, parallaxAmount);

		return Element::doSerialize(hint);
	}
}