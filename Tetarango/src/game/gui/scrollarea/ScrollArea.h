#pragma once

#include <string>
#include "gui/scrollbar/ScrollBarBasic.h"
#include "ui/renderable/container/ScrollContainer.h"
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"
#include "gui/IGuiListener.h"
#include "gui/BaseGui.h"

namespace AWGame
{

	class ScrollArea: public BaseGui, public AW::Container, public IGuiListener
	{
		bool scrollResolutionsDownPressed = false, scrollResolutionsUpPressed = false;
		std::shared_ptr<AW::ScrollContainer> container;
		std::shared_ptr<ScrollBarBasic> scroller;
		double getScrollMaxLimit();

	public:
		ScrollArea();

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onLayoutChildren();
		void onEnterFrame(double frameTime);

		void add(std::shared_ptr<GameObject> ao);
		void remove(std::shared_ptr<GameObject> ao);

		void setEnabled(bool flag);

		void setScrollerEnabled(bool enabled);
		bool getScrollerEnabled();

		void setScrollerVisible(bool visible);
		bool getScrollerVisible();

		void setScrollerXOffset(double offset);
		double getScrollerXOffset();

		void setScrollerYOffset(double offset);
		double getScrollerYOffset();

		void setScrollerHeight(double height);
		double getScrollerHeight();

		void setScrollerWidth(double width);
		double getScollerWidth();

		void setScrollAmount(double amount);
		double getScollAmount();

		void setMouseWheenEnabled(bool flag);
		void onMouseWheel(int x, int y);

		void onScrollBarScroll(int id, double position);
	};
}


