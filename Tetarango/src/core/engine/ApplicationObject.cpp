#include "ApplicationObject.h"

#include "util/VectorHelper.h"

namespace
{
	int nextId = 100;
	const auto renderOrderLambda = [](const std::shared_ptr<AW::ApplicationObject>& a, const std::shared_ptr<AW::ApplicationObject>& b) {return a->zIndex < b->zIndex;};
}

namespace AW
{
	ApplicationObject::ApplicationObject()
	{
		id = nextId++;
		modules = SystemModuleBundle::getModuleBundle();

		registerSerialization<ApplicationObject>();
	}

	int ApplicationObject::getNextId()
	{
		return nextId++;
	}

	void ApplicationObject::setTag(ATags flag, bool value)
	{
		tags[(unsigned int)flag] = value ? 1 : 0;
	}

	void ApplicationObject::commandChildren(void(*cmd)(std::shared_ptr<ApplicationObject>))
	{
		for (const auto child : children)
		{
			cmd(child);
		}
	}

	void ApplicationObject::activate()
	{
		active = true;
		currentActive = true;
	}

	void ApplicationObject::deactivate()
	{
		active = false;
		currentActive = false;
	}

	bool ApplicationObject::isActive()
	{
		return active;
	}

	bool ApplicationObject::shouldRebuildOnLoad()
	{
		return rebuildOnLoad;
	}

	bool ApplicationObject::shouldLayoutOnLoad()
	{
		return layoutOnLoad;
	}


	bool ApplicationObject::isAttached()
	{
		const auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			return parentPtr->isAttached();
		}

		return getTag(ATags::IsRootElement);
	}

	int ApplicationObject::getId()
	{
		return id;
	}

	bool ApplicationObject::getTag(ATags flag)
	{
		return tags[(unsigned int)flag] == 1;
	}

	void ApplicationObject::setTimeScope(TimeScope newScope)
	{
		if (timeScope == newScope)
		{
			return;
		}

		timeScope = newScope;
		onTimeScopeChanged();
	}

	TimeScope ApplicationObject::getTimeScope()
	{
		if (timeScope != TimeScope::None)
		{
			return timeScope;
		}

		const auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			return parentPtr->getTimeScope();
		}

		return TimeScope::Global;
	}

	void ApplicationObject::clearTimeScope()
	{
		timeScope = TimeScope::None;
		onTimeScopeChanged();
	}

	void ApplicationObject::add(std::shared_ptr<ApplicationObject> ao)
	{
		if (ao == nullptr)
		{
			return;
		}

		ao->parent = shared_from_this();
		children.push_back(ao);

		ao->attach();
	}

	void ApplicationObject::remove(std::shared_ptr<ApplicationObject> ao)
	{
		if (ao == nullptr)
		{
			return;
		}

		auto eleItr = std::find(children.begin(), children.end(), ao);
		if (eleItr != children.end())
		{
			children.erase(eleItr);
			ao->detach();
			ao->parent = std::weak_ptr<ApplicationObject>();
		}
	}

	void ApplicationObject::removeFromParent()
	{
		auto parentPointer = parent.lock();
		if (parentPointer == nullptr)
		{
			return;
		}

		parentPointer->remove(shared_from_this());
	}

	std::weak_ptr<ApplicationObject> ApplicationObject::getParent()
	{
		return parent;
	}

	const std::list<std::shared_ptr<ApplicationObject>>& ApplicationObject::getChildren()
	{
		return children;
	}

	const std::list<std::shared_ptr<ApplicationObject>>& ApplicationObject::getChildrenRenderOrder()
	{
		children.sort(renderOrderLambda);
		return children;
	}

	RenderPositionMode ApplicationObject::getFirstNonUnspecifiedRenderPositionMode()
	{
		const auto renderablePtr = std::dynamic_pointer_cast<Renderable>(shared_from_this());
		if (renderablePtr != nullptr && renderablePtr->renderPositionMode != RenderPositionMode::Unspecified)
		{
			return renderablePtr->renderPositionMode;
		}

		const auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			return parentPtr->getFirstNonUnspecifiedRenderPositionMode();
		}

		return RenderPositionMode::Unspecified;
	}

	void ApplicationObject::setWorldRect(Rect* r)
	{
		worldRect.x = r->x;
		worldRect.y = r->y;
		worldRect.w = r->w;
		worldRect.h = r->h;
	}

	void ApplicationObject::updateScreenRect(const RenderPackage* renderPackage)
	{
		if (renderPackage != nullptr)
		{
			double zoom = renderPackage->zoom,
				xOffset = renderPackage->xOffset,
				yOffset = renderPackage->yOffset;

			screenRect.w = worldRect.w * renderPackage->zoom;
			screenRect.h = worldRect.h * renderPackage->zoom;
			screenRect.x = worldRect.x * renderPackage->zoom;
			screenRect.y = worldRect.y * renderPackage->zoom;

			screenRect.x -= (renderPackage->cameraX * zoom) - (renderPackage->cameraX - xOffset) * zoom;
			screenRect.y -= (renderPackage->cameraY * zoom) - (renderPackage->cameraY - yOffset) * zoom;
		}
	}

	Rect* ApplicationObject::getWorldRect()
	{
		return &worldRect;
	}

	Rect* ApplicationObject::getScreenRect()
	{
		return &screenRect;
	}

	Rect* ApplicationObject::getCollisionRect()
	{
		return &worldRect;
	}

	void ApplicationObject::createChildren()
	{
		if (rebuildOnLoad || !hasCreatedChildren)
		{
			onCreateChildren();
			hasCreatedChildren = true;
			hasHydratedChildren = true;
		}
	}

	void ApplicationObject::destroyChildren()
	{
		onDestroyChildren();
		this->children.clear();

		hasCreatedChildren = false;
	}

	void ApplicationObject::rebuild()
	{
		destroyChildren();
		createChildren();
		layout();
	}

	void ApplicationObject::enterFrame(double frameTime)
	{
		const auto timeScope = getTimeScope();
		if (modules->time->getComputedTimeFactor(timeScope) == 0.0)
		{
			return;
		}

		EnterFrameListener::enterFrame(frameTime * modules->time->getTimeFactorForScope(timeScope));
	}

	void ApplicationObject::attach()
	{
		const auto parentPtr = parent.lock();
		if (parentPtr == nullptr)
		{
			return;
		}

		if (!hasBoundShaders)
		{
			onBindShaders();
			hasBoundShaders = true;
		}

		currentActive = parentPtr->active && currentActive;
		if (parentPtr->getInputEnabled() && getInputMode() != InputMode::Disabled) {
			setInputMode(InputMode::ParentEnabled);
		}

		if (!loaded || rebuildOnLoad) {

			if (!didInitialAttach)
			{
				onInitialAttach();
				didInitialAttach = true;
			}
			else
			{
				if (!children.empty())
				{
					destroyChildren();
				}
			}

			if (rebuildOnLoad || (softAddedChildren.empty() && !hasCreatedChildren))
			{
				createChildren();
				layout();
			}
			else
			{
				for (const auto child : softAddedChildren)
				{
					const auto ao = std::static_pointer_cast<ApplicationObject>(child);
					if (ao != nullptr)
					{
						add(ao);
					}
				}

				onChildrenHydrated();
				hasHydratedChildren = true;
				layout();
			}

			softAddedChildren.clear();
			loaded = true;
		}

		for (const auto collisionScope : collisionScopes)
		{
			modules->collision->registerObjectForCollisionScope(collisionScope, shared_from_this());
		}

		if (enterFrameActivated)
		{
			modules->event->registerOnEnterFrame(shared_from_this(), enterFramePriority);
		}

		commandChildren([](std::shared_ptr<ApplicationObject> ao) { ao->attach(); });
		onAttach();
	}

	void ApplicationObject::layout()
	{
		if (hasCreatedChildren && hasHydratedChildren)
		{
			onLayoutChildren();
		}
	}

	void ApplicationObject::detach()
	{
		currentActive = false;
		if (getInputMode() != InputMode::Disabled)
		{
			setInputMode(InputMode::Unspecified);
		}

		modules->collision->unregisterObject(shared_from_this());
		modules->event->unregisterOnEnterFrame(shared_from_this());

		auto parentPointer = parent.lock();
		if (parentPointer == nullptr)
		{
			return;
		}

		commandChildren([](std::shared_ptr<ApplicationObject> ao) { ao->detach(); });
		onDetach();
	}

	std::shared_ptr<Schematic> ApplicationObject::getSchematic()
	{
		return schematic;
	}

	std::shared_ptr<SerializationClient> ApplicationObject::doSerialize(SerializationHint hint)
	{
		if (!softAddedChildren.empty())
		{
			for (const auto child : softAddedChildren)
			{
				const auto ao = std::static_pointer_cast<ApplicationObject>(child);
				if (ao != nullptr)
				{
					children.push_back(ao);
				}
			}

			softAddedChildren.clear();
		}

		const auto client = serializationClient->getClient("__application_object__", hint);
		tags = std::bitset<16>(client->serializeString("tags", tags.to_string()));
		active = client->serializeBool("active", active);
		timeScope = (TimeScope)client->serializeInt("timescope", (int)timeScope);
		zIndex = client->serializeInt("z-index", zIndex);
		name = client->serializeString("name", name);

		enterFrameActivated = client->serializeBool("e-f-a", enterFrameActivated);
		enterFramePriority = client->serializeInt("e-f-p", enterFramePriority);
		hasCreatedChildren = client->serializeBool("h-c-c", hasCreatedChildren);
		setInputMode((InputMode)client->serializeInt("i-n-m", (int)getInputMode()));

		switch (hint)
		{
		case SerializationHint::SERIALIZE:
		{
			auto cScopes = std::vector<int>();
			for (const auto scope : collisionScopes) { cScopes.push_back((int)scope); }
			client->setString("c-scopes", VectorHelper::intVectorToString(cScopes));
		}
		break;
		case SerializationHint::HYDRATE:
		{
			auto scopes = client->getString("c-scopes");

			for (const auto scope : VectorHelper::stringToIntVector(scopes))
			{
				addCollisionScope((CollisionScope)scope);
			}
		}
		break;
		}

		return serializationClient;
	}

	std::vector<std::shared_ptr<ISerializable>> ApplicationObject::getSerializableChildren()
	{
		std::vector<std::shared_ptr<ISerializable>> serializableChildren;

		for (const auto child : children)
		{
			serializableChildren.push_back(child);
		}

		return serializableChildren;
	}

	void ApplicationObject::childHydrated(std::shared_ptr<ISerializable> child)
	{
		softAddedChildren.push_back(child);
	}

	bool ApplicationObject::shouldSerializeChildren()
	{
		return !rebuildOnLoad;
	}

	bool ApplicationObject::shouldSerializeSelf()
	{
		return serializationEnabled && !getTag(ATags::IsDebugElement);
	}

	bool ApplicationObject::collisionEnabled()
	{
		return currentActive;
	}

	void ApplicationObject::addCollisionScope(CollisionScope scope)
	{
		collisionScopes.insert(scope);
		modules->collision->registerObjectForCollisionScope(scope, shared_from_this());
	}

	void ApplicationObject::removeCollisionScope(CollisionScope scope)
	{
		collisionScopes.erase(scope);
	}

	int ApplicationObject::getObjectIdEnterFrame()
	{
		return id;
	}

	void ApplicationObject::enableEnterFrame(int priority)
	{
		if (isActive() && !enterFrameActivated)
		{
			enterFrameActivated = true;
			modules->event->registerOnEnterFrame(shared_from_this(), priority);
		}
	}

	void ApplicationObject::disableEnterFrame()
	{
		if (enterFrameActivated)
		{
			modules->event->unregisterOnEnterFrame(shared_from_this());
			enterFrameActivated = false;
			enterFramePriority = 0;
		}
	}

	int ApplicationObject::setTimeout(double timeoutMS)
	{
		return modules->event->registerTimeoutCallback(shared_from_this(), timeoutMS);
	}

	void ApplicationObject::setTimeout(double timeoutMS, int* timeoutIdLocation)
	{
		if (timeoutIdLocation != 0)
		{
			modules->event->unregisterTimeoutCallback(*timeoutIdLocation);
		}

		*timeoutIdLocation = setTimeout(timeoutMS);
	}
}
