#pragma once

#include "ui/renderable/primitive/Primitive.h"

namespace AWGame
{
	class Environment : public AW::Primitive
	{
		double currentGameTime = 0.0;
		void updateBackgroundGradient();

	public:
		Environment();

		void onBindShaders() override;
		void onInitialAttach() override;

		void onEnterFrame(const double& frameTime) override;
	};
}
