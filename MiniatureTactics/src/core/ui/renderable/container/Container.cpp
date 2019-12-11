#include "Container.h"

namespace
{
	const auto expandToChildrenParamName = "eXtc";
}

namespace MT
{
	Container::Container()
	{
		renderType = RenderType::Container;
		enableSerialization<Container>();
	}

	void Container::setExpandToChildren(bool flag)
	{
		serializationClient->setBool(expandToChildrenParamName, flag);
	}

	void Container::setSizeToScreenSize()
	{
		setSize(modules->screen->getWidth(), modules->screen->getHeight());
	}

	void Container::onDestroyChildren()
	{
		if (serializationClient->getBool(expandToChildrenParamName))
		{
			setSize(0, 0);
		}
	}

	std::shared_ptr<SerializationClient> Container::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__container__", hint);
		Renderable::doManualSerialize(hint, client);

		return ApplicationObject::doSerialize(hint);
	}

	void Container::add(std::shared_ptr<ApplicationObject> obj)
	{
		ApplicationObject::add(obj);

		if (!serializationClient->getBool(expandToChildrenParamName))
		{
			return;
		}

		const auto renderablePtr = std::dynamic_pointer_cast<Renderable>(obj);
		if (renderablePtr != nullptr)
		{
			const auto r = getRect();
			const auto rC = renderablePtr->getRect();

			double xDelta = rC.x + rC.w / 2.0;
			if (r.w < xDelta)
			{
				setWidth(xDelta);
			}

			double yDelta = rC.y + rC.h / 2.0;
			if (r.h < yDelta)
			{
				setHeight(yDelta);
			}
		}
	}

	void Container::setWidth(double newWidth)
	{
		Renderable::setWidth(newWidth);
		layout();
	}

	void Container::setHeight(double newHeight)
	{
		Renderable::setHeight(newHeight);
		layout();
	}
}