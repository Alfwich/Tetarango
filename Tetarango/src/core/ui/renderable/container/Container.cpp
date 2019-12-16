#include "Container.h"

#include "ui/renderable/element/Rectangle.h"

namespace
{
	const auto expandToChildrenParamName = "eXtc";
	const auto debugBgRectName = "__debug__bg__";
}

namespace AWCore
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

	bool Container::getExpandToChildren()
	{
		return serializationClient->getBool(expandToChildrenParamName, true);
	}

	void Container::setSizeToScreenSize()
	{
		setSize(modules->screen->getWidth(), modules->screen->getHeight());
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

		if (!getExpandToChildren())
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
		performAutoLayout();
	}

	void Container::setHeight(double newHeight)
	{
		Renderable::setHeight(newHeight);
		performAutoLayout();
	}

	void Container::performAutoLayout()
	{
		if (!isAutoLayingOut)
		{
			isAutoLayingOut = true;
			layout();
			isAutoLayingOut = false;
		}
	}

	void Container::doUpdateDebugChildren()
	{
		Renderable::doUpdateDebugChildren();

		const auto existingBg = findChildWithName<AWCore::Rectangle>(debugBgRectName, false);
		if (existingBg == nullptr)
		{
			const auto newBg = std::make_shared<AWCore::Rectangle>();
			newBg->name = debugBgRectName;
			newBg->setAlpha(0.25);
			newBg->setColor(AWCore::Color::random());
			newBg->zIndex = 1;
			newBg->matchSizeAndCenter(this);
			add(newBg);
		}
		else
		{
			existingBg->matchSizeAndCenter(this);
		}
	}
}