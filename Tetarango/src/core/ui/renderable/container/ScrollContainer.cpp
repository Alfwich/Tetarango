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

namespace AWCore
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

	double ScrollContainer::scrollPixels(int amount)
	{
		const auto currentScrollPosition = serializationClient->getDouble(scrollAmountParamName);
		const auto resultingPosition = currentScrollPosition + amount / getHeight();
		setScrollPosition(resultingPosition);

		return resultingPosition;
	}

	void ScrollContainer::setScrollPosition(double amount)
	{
		const auto min = serializationClient->getDouble(scrollAmountMinParamName, -1.0);
		const auto max = serializationClient->getDouble(scrollAmountMaxParamName, 1.0);
		serializationClient->setDouble(scrollAmountParamName, AWCore::NumberHelper::clamp<double>(amount, min, max));
		layout();
	}

	double ScrollContainer::getScrollPosition()
	{
		return serializationClient->getDouble(scrollAmountParamName, 0.0);
	}

	void ScrollContainer::setScrollLimits(double min, double max)
	{
		if (max < min)
		{
			return;
		}

		serializationClient->setDouble(scrollAmountMinParamName, min);
		serializationClient->setDouble(scrollAmountMaxParamName, max);
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

	void ScrollContainer::setScrollSpeed(int speedMS)
	{
		serializationClient->setInt(scrollSpeedInMsParamName, speedMS);
	}

	void ScrollContainer::setScrollAmountInPixels(int amount)
	{
		serializationClient->setInt(scrollAmountInPixelsParamName, amount);
	}

}