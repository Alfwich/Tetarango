#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/renderable/primitive/Circle.h"

namespace AWGame
{
	class Moon : public AW::Container
	{
		std::shared_ptr<AW::Element> moon, inner, inner2, backgroundEffect;

		void setBackgroundEffectFragmentShader();

	public:
		Moon();

		void onLoadResources() override;
		void onInitialAttach() override;
		void onCreateChildren() override;
		void onChildrenHydrated() override;
		void onLayoutChildren() override;
		void onEnterFrame(const double& frameTime) override;
	};
}
