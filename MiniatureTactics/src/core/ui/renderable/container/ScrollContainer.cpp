#include "ScrollContainer.h"

namespace
{
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

namespace MT
{
	ScrollContainer::ScrollContainer()
	{
		enableSerialization<ScrollContainer>();
	}

	void ScrollContainer::onInitialAttach()
	{
		Container::onInitialAttach();

		scrollTransition = modules->animation->createTransition();

		if (serializationClient->getBool(scrollContainerMouseWheelEnabledParamName))
		{
			modules->input->mouse->registerMouseWheel(weak_from_this());
		}
	}

	void ScrollContainer::onCreateChildren()
	{
		scrollContainer = std::make_shared<Container>();
		scrollContainer->name = scrollContainerName;
		scrollContainer->setExpandToChildren(true);
		add(scrollContainer);
	}

	void ScrollContainer::onChildrenHydrated()
	{
		scrollContainer = findChildWithName<Container>(scrollContainerName);
	}

	void ScrollContainer::onLayoutChildren()
	{
		const auto scrollAmount = serializationClient->getDouble(scrollAmountParamName);
		const auto scrollSpeedMs = serializationClient->getInt(scrollSpeedInMsParamName, 50);
		Rect target = scrollContainer->getRect();
		target.y = -scrollContainer->getHeight() * scrollAmount;
		scrollTransition->startTransition(scrollContainer, scrollSpeedMs, target);
	}

	void ScrollContainer::onMouseWheel(int x, int y)
	{
		const auto mouseX = modules->input->mouse->X();
		const auto rect = getScreenRect();
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
			for (const auto resolutionButton : children)
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
				scrollPixels(scrollAmount);
			}
		}
		else if (y > 0)
		{
			bool allowScroll = false;
			for (const auto resolutionButton : children)
			{
				allowScroll = resolutionButton->getScreenRect()->y < containerMin;

				if (allowScroll)
				{
					break;
				}
			}

			if (allowScroll)
			{
				scrollPixels(-scrollAmount);
			}
		}
	}

	void ScrollContainer::add(std::shared_ptr<ApplicationObject> ao)
	{
		if (ao->name == scrollContainerName)
		{
			Container::add(ao);
		}
		else if (scrollContainer != nullptr)
		{
			scrollContainer->add(ao);
			layout();
		}
	}

	void ScrollContainer::remove(std::shared_ptr<ApplicationObject> ao)
	{
		if (ao->name == scrollContainerName)
		{
			Container::remove(ao);
		}
		else if (scrollContainer != nullptr)
		{
			scrollContainer->remove(ao);
			layout();
		}
	}

	Rect* ScrollContainer::getScreenRect()
	{
		return scrollContainer->getScreenRect();
	}

	double ScrollContainer::getWidth()
	{
		return scrollContainer->getWidth();
	}

	double ScrollContainer::getHeight()
	{
		return scrollContainer->getHeight();
	}

	void ScrollContainer::scroll(double amount)
	{
		const auto currentScrollPosition = serializationClient->getDouble(scrollAmountParamName);
		setScrollPosition(currentScrollPosition + amount);
	}

	void ScrollContainer::scrollPixels(int amount)
	{
		const auto currentScrollPosition = serializationClient->getDouble(scrollAmountParamName);
		setScrollPosition(currentScrollPosition + amount / getHeight());
	}

	void ScrollContainer::setScrollPosition(double amount)
	{
		const auto min = serializationClient->getDouble(scrollAmountMinParamName, -1.0);
		const auto max = serializationClient->getDouble(scrollAmountMinParamName, 1.0);
		serializationClient->setDouble(scrollAmountParamName, MT::NumberHelper::clamp<double>(amount, min, max));
		layout();
	}

	void ScrollContainer::setScrollLimits(double min, double max)
	{
		if (max < min)
		{
			return;
		}

		serializationClient->setDouble(scrollAmountMinParamName, min);
		serializationClient->setDouble(scrollAmountMinParamName, max);
	}

	void ScrollContainer::setContainerLimits(double min, double max)
	{
		if (min < max)
		{
			return;
		}

		serializationClient->setDouble(scrollAmountContainerMinParamName, min);
		serializationClient->setDouble(scrollAmountContainerMaxParamName, max);
	}

	void ScrollContainer::setScrollAmountInPixels(int amount)
	{
		serializationClient->setInt(scrollAmountInPixelsParamName, amount);
	}

	void ScrollContainer::setMouseWheenEnabled(bool flag)
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

	void ScrollContainer::setScrollSpeed(int speedMS)
	{
		serializationClient->setInt(scrollSpeedInMsParamName, speedMS);
	}
}