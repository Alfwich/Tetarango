#include "Container.h"

#include "ui/renderable/primitive/Rectangle.h"

namespace
{
	const auto debugBgRectName = "__debug__bg__";
	const auto childBoundLimit = DBL_MAX;
}

namespace AW
{
	Container::Container()
	{
		renderMode = RenderMode::Container;
		debugColor = AW::Color(0, AW::NumberHelper::randomInt(0, 255), AW::NumberHelper::randomInt(0, 255));
		registerGameObject<Container>("container");
	}

	void Container::setSizeToScreenSize()
	{
		setSize(modules->screen->getWidth(), modules->screen->getHeight());
	}

	std::shared_ptr<SerializationClient> Container::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__container__", hint);
		Renderable::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}

	void Container::resizeSelfToChildrenAndCenterChildren()
	{
		isAutoLayingOut = true;

		Rect childrenBounds{ childBoundLimit, childBoundLimit, -childBoundLimit, -childBoundLimit };
		const auto r = getRect();
		for (const auto renderable : getChildrenOfType<Renderable>())
		{
			const auto rC = renderable->getRect();

			if (rC.x - rC.w / 2.0 < childrenBounds.x)
			{
				childrenBounds.x = rC.x - rC.w / 2.0;
			}

			if (rC.x + rC.w / 2.0 > childrenBounds.w)
			{
				childrenBounds.w = rC.x + rC.w / 2.0;
			}

			if (rC.y - rC.h / 2.0 < childrenBounds.y)
			{
				childrenBounds.y = rC.y - rC.h / 2.0;
			}

			if (rC.y + rC.h / 2.0 > childrenBounds.h)
			{
				childrenBounds.h = rC.y + rC.h / 2.0;
			}
		}

		childrenBounds.x = std::floor(childrenBounds.x);
		childrenBounds.y = std::floor(childrenBounds.y);
		childrenBounds.w = std::floor(childrenBounds.w);
		childrenBounds.h = std::floor(childrenBounds.h);

		setSize(childrenBounds.w - childrenBounds.x, childrenBounds.h - childrenBounds.y);

		for (const auto renderable : getChildrenOfType<Renderable>())
		{
			renderable->movePosition(-childrenBounds.x, -childrenBounds.y);
		}

		isAutoLayingOut = false;
	}

	void Container::setWidth(double newWidth)
	{
		Renderable::setWidth(newWidth);
		shouldAutoLayout = true;
	}

	void Container::setHeight(double newHeight)
	{
		Renderable::setHeight(newHeight);
		shouldAutoLayout = true;
	}

	void Container::performAutoLayoutIfNeeded()
	{
		if (!isAutoLayingOut && shouldAutoLayout)
		{
			isAutoLayingOut = true;
			layout();
			isAutoLayingOut = false;
			shouldAutoLayout = false;
		}
	}
}