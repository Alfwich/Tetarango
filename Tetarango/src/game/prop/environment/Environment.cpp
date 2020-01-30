#include "Environment.h"

#include "BackdropObject.h"

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
		AW::Color(0x652000ff), // Late Dusk
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

	const auto layoutUpdateThreshold = 16;
	const auto bodyHOffset = 150.0;
	const auto bodyVOffset = 50.0;
	const auto repeatingOffset = 3500.0;

	const auto backgroundObjectOffset = 14;
}

namespace AWGame
{
	Environment::Environment()
	{
		renderTextureMode = AW::RenderTextureMode::LinearWrapping;
		GORegister(Environment);
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

		environmentColor = colorA.lerp(colorB, p);
		const auto cAN = environmentColor.asNormalized();
		const auto cB = colorB.lerp(colorC, p).asNormalized();

		fragmentShader->setFloatV4IUParam("fColorA", cAN.r, cAN.g, cAN.b, cAN.a);
		fragmentShader->setFloatV4IUParam("fColorB", cB.r, cB.g, cB.b, cB.a);

		environmentColor.r = AW::NumberHelper::clamp(environmentColor.r * 2.0 + 50.0, 0.0, 255.0);
		environmentColor.g = AW::NumberHelper::clamp(environmentColor.g * 2.0 + 50.0, 0.0, 255.0);
		environmentColor.b = AW::NumberHelper::clamp(environmentColor.b * 2.0 + 50.0, 0.0, 255.0);
	}

	void Environment::updateParallaxContainers()
	{
		parallaxContainer1->setScreenPosition(getScreenHalfWidth() + parallaxAmountX * 0.15, getScreenHalfHeight() + parallaxAmountY * 0.15);
		parallaxContainer2->setScreenPosition(getScreenHalfWidth() + parallaxAmountX * 0.45, getScreenHalfHeight() + parallaxAmountY * 0.45);
		parallaxContainer3->setScreenPosition(getScreenHalfWidth() + parallaxAmountX * 0.55, getScreenHalfHeight() + parallaxAmountY * 0.55);
		farBackground->topLeftAlignSelf(0.0, 1000.0 + parallaxAmountY * 0.01);
	}

	void Environment::updateRepeatingParallaxElements()
	{
		for (const auto child : parallaxContainer1->getChildrenOfType<AW::Renderable>())
		{
			const auto xO = parallaxContainer1->getScreenX();
			while (child->getScreenX() + xO < -repeatingOffset)
			{
				child->moveScreenPosition(repeatingOffset * 2, 0.0);
			}

			while (child->getScreenX() + xO > repeatingOffset)
			{
				child->moveScreenPosition(-repeatingOffset * 2, 0.0);
			}
		}

		for (const auto child : parallaxContainer2->getChildrenOfType<AW::Renderable>())
		{
			const auto xO = parallaxContainer2->getScreenX();
			while (child->getScreenX() + xO < -repeatingOffset)
			{
				child->moveScreenPosition(repeatingOffset * 2, 0.0);
			}

			while (child->getScreenX() + xO > repeatingOffset)
			{
				child->moveScreenPosition(-repeatingOffset * 2, 0.0);
			}
		}

		for (const auto child : parallaxContainer3->getChildrenOfType<AW::Renderable>())
		{
			const auto xO = parallaxContainer3->getScreenX();
			while (child->getScreenX() + xO < -repeatingOffset)
			{
				child->moveScreenPosition(repeatingOffset * 2, 0.0);
			}

			while (child->getScreenX() + xO > repeatingOffset)
			{
				child->moveScreenPosition(-repeatingOffset * 2, 0.0);
			}
		}
	}

	void Environment::moveActiveParallaxElements(const double& frameTime)
	{
		for (const auto child : parallaxContainer2->getChildrenOfType<BackdropObject>())
		{
			child->moveScreenPosition(child->velocity.x * (frameTime / 1000.0), 0.0);
		}
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
		sun = std::make_shared<Sun>();
		sun->name = "sun";
		sun->setColor(252, 212, 64);
		sun->setScreenSize(150.0, 150.0);
		add(sun);

		moon = std::make_shared<Moon>();
		moon->name = "moon";
		moon->setColor(225, 225, 255);
		moon->setScreenSize(150.0, 150.0);
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

		for (auto x = -backgroundObjectOffset; x < backgroundObjectOffset; ++x)
		{
			const auto mtn = std::make_shared<BackdropObject>();
			const auto mtnSize = 1024;
			mtn->setBackdropType(BackdropType::Mountain1);
			mtn->setScreenSize(mtnSize, mtnSize);
			mtn->setScreenPosition(x * 600.0 + AW::NumberHelper::random(-100, 100), AW::NumberHelper::random(-200, 0.0));
			mtn->setColor(82, 182, 15);
			parallaxContainer1->add(mtn);
		}

		parallaxContainer2 = std::make_shared<AW::Container>();
		parallaxContainer2->name = "pc2";
		add(parallaxContainer2);

		for (auto x = -backgroundObjectOffset; x < backgroundObjectOffset; ++x)
		{
			const auto cld = std::make_shared<BackdropObject>();
			cld->setBackdropType(BackdropType::Cloud);
			cld->setAlpha(AW::NumberHelper::random(0.3, 0.75));
			cld->setScreenSize(512, 512);
			cld->setScreenPosition(x * 600.0 + AW::NumberHelper::random(-100, 100), AW::NumberHelper::random(-900.0, -200.0));
			parallaxContainer2->add(cld);
		}

		for (auto x = -backgroundObjectOffset; x < backgroundObjectOffset; ++x)
		{
			const auto mtn = std::make_shared<BackdropObject>();
			const auto mtnSize = 800;
			mtn->setBackdropType(BackdropType::Mountain1);
			mtn->setScreenSize(mtnSize, mtnSize);
			mtn->setScreenPosition(x * 600.0 + AW::NumberHelper::random(-100, 100), AW::NumberHelper::random(-150.0, 50.0));
			mtn->setColor(102, 192, 20);
			parallaxContainer2->add(mtn);
		}

		parallaxContainer3 = std::make_shared<AW::Container>();
		parallaxContainer3->name = "pc3";
		add(parallaxContainer3);

		for (auto x = -backgroundObjectOffset; x < backgroundObjectOffset; ++x)
		{
			const auto mtn = std::make_shared<BackdropObject>();
			const auto mtnSize = 650;
			mtn->setBackdropType(BackdropType::Mountain1);
			mtn->setScreenSize(mtnSize, mtnSize);
			mtn->setScreenPosition(x * 600.0 + AW::NumberHelper::random(-100, 100), AW::NumberHelper::random(-100, 50.0));
			mtn->setColor(122, 212, 40);
			parallaxContainer3->add(mtn);
		}
	}

	void Environment::onLayoutChildren()
	{
		updateParallaxContainers();
		updateRepeatingParallaxElements();
		updateBackgroundGradient();
		updateBodies();
	}

	void Environment::onChildrenHydrated()
	{
		sun = findChildWithName<Sun>("sun");
		moon = findChildWithName<Moon>("moon");
		parallaxContainer1 = findChildWithName<AW::Container>("pc1");
		parallaxContainer2 = findChildWithName<AW::Container>("pc2");
		parallaxContainer3 = findChildWithName<AW::Container>("pc3");
		farBackground = findChildWithName<AW::Rectangle>("fb");
	}

	void Environment::onEnterFrame(const double& frameTime)
	{
		moveActiveParallaxElements(frameTime);
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
		updateParallaxContainers();
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