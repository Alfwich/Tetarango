#include "Environment.h"

namespace
{
	const std::vector<AW::Color> colors = {
		AW::Color(0x005086ff),
		AW::Color(0xf69649ff),
		AW::Color(0x318fb5ff),
		AW::Color(0x65bbe3ff),
		AW::Color(0x73c8f1ff),
		AW::Color(0x73c8f1ff),
		AW::Color(0x65bbe3ff),
		AW::Color(0xea4b00ff),
		AW::Color(0x61629eff),
		AW::Color(0x1b1722ff),
		AW::Color(0x0e1425ff),
		AW::Color(0x0e1425ff)
		/*
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 255, 0, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 255, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 0, 255)
		*/
	};

	const auto dayLengthInSecondsParamKey = "env-length";
	const auto currentTimeParamKey = "env-ct";
	const auto noiseTextureName = "noise-solid-512";
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
		const auto colorA = colors[(int)std::floor((int)(currentGameTimeSeconds) % colors.size())];
		const auto colorB = colors[(int)std::floor(((int)(currentGameTimeSeconds)+1) % colors.size())];
		const auto colorC = colors[(int)std::floor(((int)(currentGameTimeSeconds)+2) % colors.size())];

		const auto cA = colorA.lerp(colorB, p).asNormalized();
		const auto cB = colorB.lerp(colorC, p).asNormalized();

		fragmentShader->setFloatV4IUParam("fColorA", cA.r, cA.g, cA.b, cA.a);
		fragmentShader->setFloatV4IUParam("fColorB", cB.r, cB.g, cB.b, cB.a);
	}

	void Environment::updateGameTime(const double& frameTime)
	{
		currentGameTime += (frameTime / dayLengthInSeconds) * colors.size();
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
		updateBackgroundGradient();
	}

	void Environment::onEnterFrame(const double& frameTime)
	{
		updateGameTime(frameTime);
		updateBackgroundGradient();
	}

	void Environment::setLengthOfDayInSeconds(double length)
	{
		dayLengthInSeconds = AW::NumberHelper::clamp(length, 1.0, DBL_MAX);
	}

	std::shared_ptr<AW::SerializationClient> Environment::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__env__", hint);

		currentGameTime = client->serializeDouble(currentTimeParamKey, currentGameTime);
		dayLengthInSeconds = client->serializeDouble(dayLengthInSecondsParamKey, dayLengthInSeconds);

		return Element::doSerialize(hint);
	}
}