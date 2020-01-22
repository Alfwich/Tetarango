#pragma once

#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Circle.h"
#include "ui/renderable/container/Container.h"

namespace AWGame
{
	class Environment : public AW::Element
	{
		double currentGameTime = 0.0, dayLengthInSeconds = 30.0, parallaxAmount = 0.0;
		void updateGameTime(const double& frameTime);

		void updateBackgroundGradient();
		void updateParallaxContainer();
		void updateBodies();

		std::shared_ptr<AW::Circle> sun, moon;
		std::shared_ptr<AW::Timer> layoutUpdateTimer;

		std::shared_ptr<AW::Container> parallaxContainer;

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
		void setParallaxAmount(double amount);

		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint) override;
	};
}
