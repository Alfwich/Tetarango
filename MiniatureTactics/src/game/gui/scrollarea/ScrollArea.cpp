#include "ScrollArea.h"
#include "gui/Guis.h"

namespace
{
	const auto containerId = "scroll-area-container";
	const auto scrollerId = "scroll-area-scroller";

	const auto enabledParamName = "sb-i-e";
	const auto scrollContainerMouseWheelEnabledParamName = "sc-mw-e";

	const auto scrollerEnabledParam = "sa-s-i-e";
	const auto scrollerVisibleParam = "sa-s-i-v";
	const auto scrollerWidthParam = "sa-sw";
	const auto scrollerHeightParam = "sa-sh";
	const auto scrollerXOffsetParam = "sa-x-o";
	const auto scrollerYOffsetParam = "sa-y-o";
}

namespace MTGame
{
	ScrollArea::ScrollArea() : BaseGui(GuiScrollArea::ScrollArea)
	{
		enableSerialization<ScrollArea>();
	}

	void ScrollArea::setEnabled(bool flag)
	{
		serializationClient->setBool(enabledParamName, flag);
	}

	void ScrollArea::onCreateChildren()
	{
		container = std::make_shared<MT::ScrollContainer>();
		container->name = containerId;
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
		container = findChildWithName<MT::ScrollContainer>(containerId);
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
		scroller->toRightOf(container, scroller->getHalfWidth() + getScrollerXOffset(), -container->getHalfHeight() + scroller->getHeight() + (scroller->getScrollerHeight() / 2.0) + getScrollerYOffset());
	}

	void ScrollArea::onEnterFrame(double frameTime)
	{
		if (scroller->visible)
		{
			scroller->setScrollPosition(container->getScrollPosition());
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
			matchSize(container);
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
		}
	}

	void ScrollArea::onMouseWheel(int x, int y)
	{
		const auto mouseX = modules->input->mouse->X();
		const auto rect = container->getScreenRect();
		if (mouseX < rect->x || mouseX > rect->x + rect->w)
		{
			return;
		}

		const auto containerMin = 0.0;
		const auto containerMax = modules->screen->getHeight();
		const auto scrollAmount = 50;
		if (y < 0)
		{
			bool allowScroll = false;
			for (const auto resolutionButton : container->getChildren())
			{
				const auto screenRect = resolutionButton->getScreenRect();
				allowScroll = screenRect->y + screenRect->h > containerMax;

				if (allowScroll)
				{
					break;
				}
			}

			if (allowScroll)
			{
				container->scrollPixels(scrollAmount);
			}
		}
		else if (y > 0)
		{
			bool allowScroll = false;
			for (const auto resolutionButton : container->getChildren())
			{
				allowScroll = resolutionButton->getScreenRect()->y < containerMin;

				if (allowScroll)
				{
					break;
				}
			}

			if (allowScroll)
			{
				container->scrollPixels(-scrollAmount);
			}
		}
	}

	void ScrollArea::onScrollBarScroll(double position)
	{
		container->setScrollPosition(position);
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
		return serializationClient->getDouble(scrollerHeightParam, 400.0);
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
		return serializationClient->getDouble(scrollerHeightParam, 15.0);
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
