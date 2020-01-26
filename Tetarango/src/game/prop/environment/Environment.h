#pragma once

#include "ui/renderable/element/Element.h"
#include "ui/renderable/container/Container.h"
#include "prop/environment/Sun.h"
#include "prop/environment/Moon.h"

namespace AWGame
{
	class Environment : public AW::Element, public AW::ICameraListener
	{
		AW::Color environmentColor;
		double currentGameTime = 0.0, dayLengthInSeconds = 30.0, parallaxAmountX = 0.0, parallaxAmountY = 0.0;
		void updateGameTime(const double& frameTime);

		void updateBackgroundGradient();
		void updateParallaxContainers();
		void updateRepeatingParallaxElements();
		void moveActiveParallaxElements(const double& frameTime);
		void updateBodies();

		std::shared_ptr<Sun> sun;
		std::shared_ptr<Moon> moon;
		std::shared_ptr<AW::Timer> layoutUpdateTimer;

		std::shared_ptr<AW::Container> parallaxContainer1, parallaxContainer2, parallaxContainer3;
		std::shared_ptr<AW::Rectangle> farBackground;

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
		void setParallaxAmount(double amountX, double amountY);

		const AW::Color& getEnvironmentColor();

		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint) override;
	};
}
