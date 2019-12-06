#include "ScrollArea.h"
#include "gui/Guis.h"

namespace
{
	const auto containerId = "scroll-area-container";
	const auto scrollerId = "scroll-area-scroller";

	const auto enabledParamName = "scrollbar-basic-is-enabled";

	const auto scrollContainerName = "scroll-container";
	const auto scrollAmountParamName = "scroll-container-amount";
	const auto scrollAmountMinParamName = "scroll-container-amount-min";
	const auto scrollAmountMaxParamName = "scroll-container-amount-max";
	const auto scrollAmountContainerMinParamName = "scroll-container-amount-min";
	const auto scrollAmountContainerMaxParamName = "scroll-container-amount-max";
	const auto scrollAmountInPixelsParamName = "scroll-container-amount-in-pixels";
	const auto scrollContainerMouseWheelEnabledParamName = "scroll-container-mouse-wheel-enabled";
	const auto scrollSpeedInMsParamName = "scroll-container-speed-ms";
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
		scroller->setSize(15.0, 400.0);
		scroller->zIndex = 1;
		add(scroller);
	}

	void ScrollArea::onChildrenHydrated()
	{
		container = findChildWithName<MT::ScrollContainer>(containerId);
		scroller = findChildWithName<ScrollBarBasic>(scrollerId);
	}

	void ScrollArea::onInitialAttach()
	{
		modules->input->mouse->registerMouseWheel(weak_from_this());
		enableEnterFrame(-1);
	}

	void ScrollArea::onLayoutChildren()
	{
		container->centerAlignSelf();
		scroller->toRightOf(container, scroller->getHalfWidth() + 2, -container->getHalfHeight() + scroller->getHeight() + (scroller->getScrollerHeight() / 2.0));
	}

	void ScrollArea::onEnterFrame(double frameTime)
	{
		scroller->setScrollPosition(container->getScrollPosition());
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

		const auto containerMin = serializationClient->getDouble(scrollAmountContainerMinParamName, 0.0);
		const auto containerMax = serializationClient->getDouble(scrollAmountContainerMaxParamName, modules->screen->getHeight());
		const auto scrollAmount = serializationClient->getInt(scrollAmountInPixelsParamName, 50);
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
