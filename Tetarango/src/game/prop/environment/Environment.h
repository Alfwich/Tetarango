#pragma once

#include "ui/renderable/element/Element.h"

namespace AWGame
{
	class Environment : public AW::Element
	{
		double currentGameTime = 0.0, dayLengthInSeconds = 300.0;
		void updateBackgroundGradient();
		void updateGameTime(const double& frameTime);

	public:
		Environment();

		void onLoadResources() override;

		void onBindShaders() override;
		void onInitialAttach() override;
		void onEnterFrame(const double& frameTime) override;

		void setLengthOfDayInSeconds(double length);

		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint) override;
	};
}
