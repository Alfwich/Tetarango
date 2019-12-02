#pragma once

#include "Container.h"

namespace MT
{
	class ScrollContainer : public Container
	{
		bool scrollResolutionsDownPressed = false, scrollResolutionsUpPressed = false;
		std::shared_ptr<Transition> scrollTransition;
		std::shared_ptr<Container> scrollContainer;

	public:
		ScrollContainer();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onLayoutChildren();
		void onMouseWheel(int x, int y);

		void add(std::shared_ptr<ApplicationObject> ao);
		void remove(std::shared_ptr<ApplicationObject> ao);

		Rect* getScreenRect();
		double getWidth();
		double getHeight();

		void scroll(double amount);
		void scrollPixels(int amount);
		void setScrollPosition(double amount);

		void setScrollLimits(double min, double max);
		void setContainerLimits(double min, double max);
		void setScrollAmountInPixels(int amount);
		void setMouseWheenEnabled(bool flag);
		void setScrollSpeed(int speedMS = 50);
	};
}
