#include "Environment.h"

namespace
{
	const std::vector<AW::Color> colors = {
		AW::Color(255, 0, 0, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 255, 0, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 255, 255),
		AW::Color(0, 0, 0, 255),
		AW::Color(0, 0, 0, 255)
	};
}

namespace AWGame
{
	Environment::Environment()
	{
		GORegister(Environment);
	}

	void Environment::updateBackgroundGradient()
	{
		const auto p = ((currentGameTime / 1000.0) - std::floor(currentGameTime / 1000.0)) / 1.0;
		const auto colorA = colors[(int)std::floor((int)(currentGameTime / 1000.0) % colors.size())];
		const auto colorB = colors[(int)std::floor(((int)(currentGameTime / 1000.0) + 1) % colors.size())];
		const auto colorC = colors[(int)std::floor(((int)(currentGameTime / 1000.0) + 2) % colors.size())];

		const auto cA = colorA.lerp(colorB, p).asNormalized();
		const auto cB = colorB.lerp(colorC, p).asNormalized();

		fragmentShader->setFloatV4IUParam("fColorA", cA.r, cA.g, cA.b, cA.a);
		fragmentShader->setFloatV4IUParam("fColorB", cB.r, cB.g, cB.b, cB.a);
	}

	void Environment::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-color", "f-vgradient" });
	}

	void Environment::onInitialAttach()
	{
		enableEnterFrame();
		updateBackgroundGradient();
	}

	void Environment::onEnterFrame(const double& frameTime)
	{
		currentGameTime += frameTime;
		updateBackgroundGradient();
	}
}