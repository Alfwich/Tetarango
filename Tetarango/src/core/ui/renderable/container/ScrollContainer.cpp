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

namespace AW
{
	ScrollContainer::ScrollContainer()
	{
		GORegister(ScrollContainer);
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
		add(scrollContainer);
	}

	void ScrollContainer::onChildrenHydrated()
	{
		scrollContainer = findChildWithName<Container>(scrollContainerName);
	}

	void ScrollContainer::onLayoutChildren()
	{
		scrollContainer->resizeSelfToChildrenAndCenterChildren();
		scrollContainer->setScreenX(scrollContainer->getScreenHalfWidth());
		const auto scrollAmount = getScrollPosition();
		const auto scrollSpeedMs = serializationClient->getInt(scrollSpeedInMsParamName, 50);
		Rect target = scrollContainer->getRect() + Rect(0, scrollContainer->getScreenHalfHeight(), 0, 0);
		target.y = -scrollContainer->getScreenHeight() * scrollAmount + scrollContainer->getScreenHalfHeight();
		scrollTransition->startTransition(scrollContainer, doNextLayoutInstantly ? 0.0 : scrollSpeedMs, target);
		doNextLayoutInstantly = false;
	}

	void ScrollContainer::add(std::shared_ptr<GameObject> ao)
	{
		if (ao->name == scrollContainerName)
		{
			Container::add(ao);
		}
		else if (scrollContainer != nullptr)
		{
			scrollContainer->add(ao);
			doNextLayoutInstantly = true;
			layout();
		}
	}

	void ScrollContainer::remove(std::shared_ptr<GameObject> ao)
	{
		if (ao->name == scrollContainerName)
		{
			Container::remove(ao);
		}
		else if (scrollContainer != nullptr)
		{
			scrollContainer->remove(ao);
			doNextLayoutInstantly = true;
			layout();
		}
	}

	const Rect* ScrollContainer::getScreenRect() const
	{
		return scrollContainer->getScreenRect();
	}

	double ScrollContainer::getScreenWidth() const
	{
		return scrollContainer->getScreenWidth();
	}

	double ScrollContainer::getScreenHeight() const
	{
		return scrollContainer->getScreenHeight();
	}

	void ScrollContainer::scroll(double amount)
	{
		const auto currentScrollPosition = getScrollPosition();
		setScrollPosition(currentScrollPosition + amount);
	}

	double ScrollContainer::scrollPixels(int amount)
	{
		const auto currentScrollPosition = getScrollPosition();
		const auto resultingPosition = currentScrollPosition + amount / getScreenHeight();
		setScrollPosition(resultingPosition);

		return resultingPosition;
	}

	void ScrollContainer::setScrollPosition(double amount)
	{
		const auto min = serializationClient->getDouble(scrollAmountMinParamName, -1.0);
		const auto max = serializationClient->getDouble(scrollAmountMaxParamName, 1.0);
		serializationClient->setDouble(scrollAmountParamName, AW::NumberHelper::clamp<double>(amount, min, max));
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