#pragma once

#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Circle.h"

namespace AWGame
{
	class Environment : public AW::Element
	{
		double currentGameTime = 0.0, dayLengthInSeconds = 30.0;
		void updateBackgroundGradient();
		void updateBodies();
		void updateGameTime(const double& frameTime);

		std::shared_ptr<AW::Circle> sun, moon;

	public:
		Environment();

		void onLoadResources() override;

		void onBindShaders() override;

		void onInitialAttach() override;
		void onCreateChildren() override;
		void onLayoutChildren() override;
		void onChildrenHydrated() override;
		void onEnterFrame(const double& frameTime) override;

		void setLengthOfDayInSeconds(double length);

		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint) override;
	};
}
