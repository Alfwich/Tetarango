#include "GameObject.h"

#include "util/VectorHelper.h"

namespace
{
	int nextId = 100;
	const auto renderOrderLambda = [](const std::shared_ptr<AW::GameObject>& a, const std::shared_ptr<AW::GameObject>& b) {return a->zIndex < b->zIndex;};
}

namespace AW
{
	GameObject::GameObject()
	{
		id = nextId++;
		modules = SystemModuleBundle::getModuleBundle();

		registerSerialization<GameObject>();
	}

	int GameObject::getNextId()
	{
		return nextId++;
	}

	void GameObject::setTag(GTags flag, bool value)
	{
		tags[(unsigned int)flag] = value ? 1 : 0;
	}

	void GameObject::commandChildren(void(*cmd)(std::shared_ptr<GameObject>))
	{
		for (const auto child : children)
		{
			cmd(child);
		}
	}

	void GameObject::activate()
	{
		active = true;
		currentActive = true;
	}

	void GameObject::deactivate()
	{
		active = false;
		currentActive = false;
	}

	bool GameObject::isActive()
	{
		return active;
	}

	bool GameObject::shouldRebuildOnLoad()
	{
		return rebuildOnLoad;
	}

	bool GameObject::shouldLayoutOnLoad()
	{
		return layoutOnLoad;
	}


	bool GameObject::isAttached()
	{
		const auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			return parentPtr->isAttached();
		}

		return getTag(GTags::IsRootElement);
	}

	int GameObject::getId()
	{
		return id;
	}

	bool GameObject::getTag(GTags flag)
	{
		return tags[(unsigned int)flag] == 1;
	}

	void GameObject::setTimeScope(TimeScope newScope)
	{
		if (timeScope == newScope)
		{
			return;
		}

		timeScope = newScope;
		onTimeScopeChanged();
	}

	TimeScope GameObject::getTimeScope()
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

	void GameObject::clearTimeScope()
	{
		timeScope = TimeScope::None;
		onTimeScopeChanged();
	}

	void GameObject::add(std::shared_ptr<GameObject> ao)
	{
		if (ao == nullptr)
		{
			return;
		}

		ao->parent = shared_from_this();
		children.push_back(ao);

		ao->attach();
	}

	void GameObject::remove(std::shared_ptr<GameObject> ao)
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
			ao->parent = std::weak_ptr<GameObject>();
		}
	}

	void GameObject::removeFromParent()
	{
		auto parentPointer = parent.lock();
		if (parentPointer == nullptr)
		{
			return;
		}

		parentPointer->remove(shared_from_this());
	}

	std::weak_ptr<GameObject> GameObject::getParent()
	{
		return parent;
	}

	const std::list<std::shared_ptr<GameObject>>& GameObject::getChildren()
	{
		return children;
	}

	const std::list<std::shared_ptr<GameObject>>& GameObject::getChildrenRenderOrder()
	{
		children.sort(renderOrderLambda);
		return children;
	}

	void GameObject::createChildren()
	{
		if (rebuildOnLoad || !hasCreatedChildren)
		{
			onCreateChildren();
			hasCreatedChildren = true;
			hasHydratedChildren = true;
		}
	}

	void GameObject::destroyChildren()
	{
		onDestroyChildren();
		this->children.clear();

		hasCreatedChildren = false;
	}

	void GameObject::rebuild()
	{
		destroyChildren();
		createChildren();
		layout();
	}

	void GameObject::enterFrame(const double& frameTime)
	{
		const auto timeScope = getTimeScope();
		const auto timeFactor = modules->time->getTimeFactorForScope(timeScope);
		if (timeFactor * modules->time->getGlobalTimeFactor() == 0.0)
		{
			return;
		}

		if (timeFactor != 1.0)
		{
			const auto timeFactoredFrameTime = frameTime * timeFactor;
			EnterFrameListener::enterFrame(timeFactoredFrameTime);
		}
		else
		{

			EnterFrameListener::enterFrame(frameTime);
		}
	}

	void GameObject::attach()
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
					const auto ao = std::static_pointer_cast<GameObject>(child);
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

		commandChildren([](std::shared_ptr<GameObject> ao) { ao->attach(); });
		onAttach();
	}

	void GameObject::layout()
	{
		if (hasCreatedChildren && hasHydratedChildren)
		{
			onLayoutChildren();
		}
	}

	void GameObject::detach()
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

		commandChildren([](std::shared_ptr<GameObject> ao) { ao->detach(); });
		onDetach();
	}

	std::shared_ptr<Schematic> GameObject::getSchematic()
	{
		return schematic;
	}

	std::shared_ptr<SerializationClient> GameObject::doSerialize(SerializationHint hint)
	{
		if (!softAddedChildren.empty())
		{
			for (const auto child : softAddedChildren)
			{
				const auto ao = std::static_pointer_cast<GameObject>(child);
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

	std::vector<std::shared_ptr<ISerializable>> GameObject::getSerializableChildren()
	{
		std::vector<std::shared_ptr<ISerializable>> serializableChildren;

		for (const auto child : children)
		{
			serializableChildren.push_back(child);
		}

		return serializableChildren;
	}

	void GameObject::childHydrated(std::shared_ptr<ISerializable> child)
	{
		softAddedChildren.push_back(child);
	}

	bool GameObject::shouldSerializeChildren()
	{
		return !rebuildOnLoad;
	}

	bool GameObject::shouldSerializeSelf()
	{
		return serializationEnabled && !getTag(GTags::IsDebugElement);
	}

	bool GameObject::collisionEnabled()
	{
		return currentActive;
	}

	void GameObject::addCollisionScope(CollisionScope scope)
	{
		collisionScopes.insert(scope);
		modules->collision->registerObjectForCollisionScope(scope, shared_from_this());
	}

	void GameObject::removeCollisionScope(CollisionScope scope)
	{
		collisionScopes.erase(scope);
	}

	int GameObject::getObjectIdEnterFrame()
	{
		return id;
	}

	void GameObject::enableEnterFrame(int priority)
	{
		if (isActive() && !enterFrameActivated)
		{
			enterFrameActivated = true;
			modules->event->registerOnEnterFrame(shared_from_this(), priority);
		}
	}

	void GameObject::disableEnterFrame()
	{
		if (enterFrameActivated)
		{
			modules->event->unregisterOnEnterFrame(shared_from_this());
			enterFrameActivated = false;
			enterFramePriority = 0;
		}
	}

	int GameObject::setTimeout(double timeoutMS)
	{
		return modules->event->registerTimeoutCallback(shared_from_this(), timeoutMS);
	}

	void GameObject::setTimeout(double timeoutMS, int* timeoutIdLocation)
	{
		if (timeoutIdLocation != 0)
		{
			modules->event->unregisterTimeoutCallback(*timeoutIdLocation);
		}

		*timeoutIdLocation = setTimeout(timeoutMS);
	}
}
