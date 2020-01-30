#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Circle.h"

namespace AWGame
{
	class Sun : public AW::Container
	{
		double currentTime = 0.0;
		std::shared_ptr<AW::Element> sun, inner, inner2, backgroundEffect;

		void setBackgroundEffectFragmentShader();
	public:
		Sun();

		void onLoadResources() override;
		void onInitialAttach() override;
		void onCreateChildren() override;
		void onChildrenHydrated() override;
		void onLayoutChildren() override;
		void onEnterFrame(const double& frameTime) override;
	};
}
