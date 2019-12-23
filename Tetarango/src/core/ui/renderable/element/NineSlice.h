#pragma once

#include "ui/renderable/element/Animated.h"
#include "ui/renderable/container/Container.h"

namespace AW
{

	class NineSlice : public Animated
	{
	public:
		NineSlice();

		void onBindShaders();
		void onInitialAttach();
		void onLayoutChildren();

		void setWidth(double width);
		void setHeight(double height);

		void setCurrentAnimation(std::string animationName);

		void setCornerSize(double cornerSize);
		double getCornerSize();
	};
}
