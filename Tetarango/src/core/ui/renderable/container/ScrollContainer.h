#pragma once

#include "Container.h"

namespace AW
{
	class ScrollContainer : public Container
	{
		bool doNextLayoutInstantly = false;
		std::shared_ptr<Transition> scrollTransition;
		std::shared_ptr<Container> scrollContainer;

	public:
		ScrollContainer();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onLayoutChildren();

		void add(std::shared_ptr<GameObject> ao);
		void remove(std::shared_ptr<GameObject> ao);

		const Rect* getScreenRect();
		double getScreenWidth();
		double getScreenHeight();

		void scroll(double amount);
		double scrollPixels(int amount);
		void setScrollPosition(double amount);
		double getScrollPosition();

		void setScrollLimits(double min, double max);
		void setContainerLimits(double min, double max);
		void setScrollSpeed(int speedMS = 50);
		void setScrollAmountInPixels(int amount);
	};
}
