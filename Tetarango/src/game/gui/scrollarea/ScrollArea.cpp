#include "ScrollArea.h"
#include "gui/Guis.h"
#include "ui/renderable/element/Rectangle.h"

namespace
{
	const auto containerId = "scroll-area-container";
	const auto scrollerId = "scroll-area-scroller";

	const auto enabledParamName = "sb-i-e";
	const auto scrollerExpandToChildrenParamName = "sb-x-t-c";
	const auto scrollContainerMouseWheelEnabledParamName = "sc-mw-e";

	const auto scrollerEnabledParam = "sa-s-i-e";
	const auto scrollerVisibleParam = "sa-s-i-v";
	const auto scrollerWidthParam = "sa-sw";
	const auto scrollerHeightParam = "sa-sh";
	const auto scrollerXOffsetParam = "sa-x-o";
	const auto scrollerYOffsetParam = "sa-y-o";
	const auto scrollerScrollAmountParam = "sa-s-a";
}

namespace AWGame
{

	ScrollArea::ScrollArea() : BaseGui(GuiScrollArea::ScrollArea)
	{
		enableSerialization<ScrollArea>();
	}

	double ScrollArea::getScrollMaxLimit()
	{
		const auto containerHeight = container->getHeight();
		const auto myHeight = getHeight();

		return (containerHeight - myHeight) / containerHeight;
	}

	void ScrollArea::setEnabled(bool flag)
	{
		serializationClient->setBool(enabledParamName, flag);
	}

	void ScrollArea::onCreateChildren()
	{
		container = std::make_shared<AWCore::ScrollContainer>();
		container->name = containerId;
		container->setExpandToChildren(true);
		add(container);

		scroller = std::make_shared<ScrollBarBasic>();
		scroller->name = scrollerId;
		scroller->scrollListener = weak_from_this();
		scroller->setSize(getScollerWidth(), getScrollerHeight());
		scroller->zIndex = 1;
		scroller->visible = getScrollerVisible();
		scroller->setInputEnabled(getScrollerEnabled());
		add(scroller);
	}

	void ScrollArea::onChildrenHydrated()
	{
		container = findChildWithName<AWCore::ScrollContainer>(containerId);
		scroller = findChildWithName<ScrollBarBasic>(scrollerId);
		scroller->scrollListener = weak_from_this();
	}

	void ScrollArea::onInitialAttach()
	{
		modules->input->mouse->registerMouseWheel(weak_from_this());
		enableEnterFrame(-1);
	}

	void ScrollArea::onLayoutChildren()
	{
		container->centerAlignSelf();

		const auto containerHeight = container->getHeight();
		const auto myHeight = getHeight();
		const auto shouldAllowScroll = containerHeight > myHeight;
		if (shouldAllowScroll)
		{
			container->setScrollLimits(0.0, getScrollMaxLimit());
		}
		else
		{
			container->setScrollLimits(0.0, 0.0);
		}

		if (getScrollerVisible())
		{
			if (shouldAllowScroll)
			{
				scroller->toRightTopOf(container);
				scroller->setScrollerHeight(myHeight / containerHeight);
				scroller->visible = getScrollerVisible();
				scroller->setScrollPositionInstantly(container->getScrollPosition());
			}
			else
			{
				scroller->visible = false;
			}
		}
	}

	void ScrollArea::onEnterFrame(double frameTime)
	{
		if (scroller->visible)
		{
			scroller->setScrollPosition(container->getScrollPosition() / getScrollMaxLimit());
		}
	}

	void ScrollArea::add(std::shared_ptr<ApplicationObject> ao)
	{
		if (ao->name == containerId || ao->name == scrollerId)
		{
			Container::add(ao);
		}
		else
		{
			container->add(ao);

			if (getExpandToChildren())
			{
				matchSize(container);
			}

			layout();
		}
	}

	void ScrollArea::remove(std::shared_ptr<ApplicationObject> ao)
	{
		if (ao->name == containerId || ao->name == scrollerId)
		{
			Container::remove(ao);
		}
		else
		{
			container->remove(ao);

			if (getExpandToChildren())
			{
				matchSize(container);
			}

			layout();
		}
	}

	void ScrollArea::onMouseWheel(int x, int y)
	{
		const auto mouseX = modules->input->mouse->X();
		const auto mouseY = modules->input->mouse->Y();
		const auto rect = getScreenRect();
		if (mouseX < rect->x || mouseX > rect->x + rect->w || mouseY < rect->y || mouseY > rect->y + rect->h)
		{
			return;
		}

		const auto scrollAmount = getScollAmount();
		if (y < 0)
		{
			container->scrollPixels(scrollAmount);
		}
		else if (y > 0)
		{
			container->scrollPixels(-scrollAmount);
		}
	}

	void ScrollArea::onScrollBarScroll(int id, double position)
	{
		container->setScrollPosition(position * getScrollMaxLimit());
	}

	void ScrollArea::setScrollerEnabled(bool enabled)
	{
		serializationClient->setBool(scrollerEnabledParam, enabled);
		if (scroller != nullptr)
		{
			scroller->setInputEnabled(enabled);
		}
	}

	bool ScrollArea::getScrollerEnabled()
	{
		return serializationClient->getBool(scrollerEnabledParam, true);
	}

	void ScrollArea::setScrollerVisible(bool visible)
	{
		serializationClient->setBool(scrollerVisibleParam, visible);
		if (scroller != nullptr)
		{
			scroller->visible = visible;
		}
	}

	bool ScrollArea::getScrollerVisible()
	{
		return serializationClient->getBool(scrollerVisibleParam, true);
	}

	void ScrollArea::setScrollerXOffset(double offset)
	{
		serializationClient->setDouble(scrollerXOffsetParam, offset);
		if (scroller != nullptr)
		{
			layout();
		}
	}

	double ScrollArea::getScrollerXOffset()
	{
		return serializationClient->getDouble(scrollerXOffsetParam, 2.0);
	}

	void ScrollArea::setScrollerYOffset(double offset)
	{
		serializationClient->setDouble(scrollerYOffsetParam, offset);
		if (scroller != nullptr)
		{
			layout();
		}
	}

	double ScrollArea::getScrollerYOffset()
	{
		return serializationClient->getDouble(scrollerYOffsetParam, 0.0);
	}

	void ScrollArea::setScrollerHeight(double height)
	{
		serializationClient->setDouble(scrollerHeightParam, height);

		if (scroller != nullptr)
		{
			scroller->setSize(getScollerWidth(), getScrollerHeight());
			layout();
		}
	}

	double ScrollArea::getScrollerHeight()
	{
		return std::min(getHeight(), serializationClient->getDouble(scrollerHeightParam, 400.0));
	}

	void ScrollArea::setScrollerWidth(double width)
	{
		serializationClient->setDouble(scrollerWidthParam, width);
		if (scroller != nullptr)
		{
			scroller->setSize(getScollerWidth(), getScrollerHeight());
			layout();
		}
	}

	double ScrollArea::getScollerWidth()
	{
		return serializationClient->getDouble(scrollerWidthParam, 15.0);
	}

	void ScrollArea::setScrollAmount(double amount)
	{
		serializationClient->setDouble(scrollerScrollAmountParam, amount);
	}

	double ScrollArea::getScollAmount()
	{
		return serializationClient->getDouble(scrollerScrollAmountParam, 50.0);
	}

	void ScrollArea::setMouseWheenEnabled(bool flag)
	{
		const auto current = serializationClient->getBool(scrollContainerMouseWheelEnabledParamName);

		if (current != flag)
		{
			serializationClient->setBool(scrollContainerMouseWheelEnabledParamName, flag);
		}

		if (flag)
		{
			modules->input->mouse->registerMouseWheel(weak_from_this());
		}
		else
		{
			modules->input->mouse->unregisterMouseWheel(shared_from_this());
		}
	}
}
