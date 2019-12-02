#pragma once

#include "Container.h"

namespace MT
{
	class ScrollContainer : public Container
	{
		std::shared_ptr<Transition> scrollTransition;
		std::shared_ptr<Container> scrollContainer;
	public:
		ScrollContainer();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onLayoutChildren();

		void add(std::shared_ptr<ApplicationObject> ao);
		void remove(std::shared_ptr<ApplicationObject> ao);

		Rect* getScreenRect();
		double getWidth();
		double getHeight();

		void scroll(double amount);
		void scrollPixels(int amount);
		void setScrollPosition(double amount);
		void setScrollLimits(double min = 0.0, double max = 1.0);
	};
}
