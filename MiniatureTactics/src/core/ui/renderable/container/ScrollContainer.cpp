#include "ScrollContainer.h"

namespace
{
	const auto scrollAmountParamName = "scroll-amount";
	const auto scrollAmountMinParamName = "scroll-amount-min";
	const auto scrollAmountMaxParamName = "scroll-amount-max";
	const auto scrollContainerName = "scroll-container";
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
		Rect target = scrollContainer->getRect();
		target.y = -scrollContainer->getHeight() * scrollAmount;
		scrollTransition->startTransition(scrollContainer, 50.0, target);
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
}