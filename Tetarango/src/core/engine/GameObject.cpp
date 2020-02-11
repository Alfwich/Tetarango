#include "GameObject.h"

namespace
{
	int nextGameObjectId = 100;
	const auto orderLambda = [](const std::shared_ptr<AW::GameObject>& a, const std::shared_ptr<AW::GameObject>& b) { return a->zIndex < b->zIndex; };
	const auto luaImplKeyParamName = "go_lua_impl_key";

	const auto luaEnterFrameFunctionName = "AW_enterFrame";
	const auto luaOnKeyFunctionName = "AW_onKey";
	const auto luaValidGameObjectFunctionName = "AW_validGameObject";
	const auto luaCreateGameObjectFunctionName = "AW_createGameObject";
	const auto luaSetParentGameObjectFunctionName = "AW_setParentGameObject";
	const auto luaMoveToParentContainerGameObjectFunctionName = "AW_moveToParentGameObject";
	const auto luaGetChildrenGameObjectFunctionName = "AW_getChildrenGameObject";
	const auto luaGetParentGameObjectFunctionName = "AW_getParentGameObject";

	const auto luaFireEventGameObjectFunctionName = "fireEvent";
	const auto luaFireUpEventGameObjectFunctionName = "fireUpEvent";
}

namespace AW
{
	int GameObject::nextId()
	{
		return nextGameObjectId++;
	}

	int GameObject::currentBindingId()
	{
		auto now = std::chrono::system_clock::now();
		auto time = now.time_since_epoch().count();
		return (int)std::chrono::nanoseconds(time).count();
	}

	GameObject::GameObject() : id(nextId()), idString(std::to_string(id)), bindingId(currentBindingId()), modules(SystemModuleBundle::getModuleBundle())
	{
		setTag(GTags::IsActive, true);
		setTag(GTags::LayoutOnLoad, true);
		setTag(GTags::SerializationEnabled, true);
		setTag(GTags::ChildrenSorted, true);

		GORegister(GameObject);
	}

	GameObject::~GameObject()
	{
		modules->lua->unregisterBoundFunctions(getLuaBindingId());
	}

	void GameObject::setTag(GTags tag, bool value)
	{
		tags[(unsigned int)tag] = value ? 1 : 0;
	}

	bool GameObject::getTag(GTags tag)
	{
		return tags[(unsigned int)tag] == 1;
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
		setTag(GTags::IsActive, true);
		setTag(GTags::IsCurrentActive, true);
	}

	void GameObject::deactivate()
	{
		setTag(GTags::IsActive, false);
		setTag(GTags::IsCurrentActive, false);
	}

	bool GameObject::isActive()
	{
		return getTag(GTags::IsActive);
	}

	void GameObject::setShouldRebuildOnLoad(bool flag)
	{
		setTag(GTags::RebuildOnLoad, flag);
	}

	bool GameObject::shouldRebuildOnLoad()
	{
		return getTag(GTags::RebuildOnLoad);
	}

	void GameObject::setShouldLayoutOnLoad(bool flag)
	{
		setTag(GTags::LayoutOnLoad, flag);
	}

	bool GameObject::shouldLayoutOnLoad()
	{
		return getTag(GTags::LayoutOnLoad);
	}

	void GameObject::setLuaBindingsEnabled(bool flag)
	{
		if (!luaBindingsEnabled() && !getTag(GTags::HasRegisteredLuaBindings) && flag)
		{
			onBindLuaHooks();
			setTag(GTags::HasRegisteredLuaBindings, true);

			const auto implKey = serializationClient->getString(luaImplKeyParamName);
			if (currentActiveLuaImpl != implKey && !implKey.empty())
			{
				modules->lua->setObjectImplementation(getLuaBindingId(), implKey);
				currentActiveLuaImpl = implKey;
			}
		}
		else if (luaBindingsEnabled() && getTag(GTags::HasRegisteredLuaBindings) && !flag)
		{
			modules->lua->unregisterBoundFunctions(getLuaBindingId());
			setTag(GTags::HasRegisteredLuaBindings, false);
		}

		setTag(GTags::LuaBindingsEnabled, flag);
	}

	void GameObject::enableLuaBindings()
	{
		setLuaBindingsEnabled(true);
	}

	void GameObject::disableLuaBindings()
	{
		setLuaBindingsEnabled(false);
	}

	void GameObject::setLuaImplementation(const std::string& implKey)
	{
		serializationClient->setString(luaImplKeyParamName, implKey);
		if (currentActiveLuaImpl != implKey && getTag(GTags::HasRegisteredLuaBindings))
		{
			modules->lua->setObjectImplementation(getLuaBindingId(), implKey);
			currentActiveLuaImpl = implKey;
		}
		enableLuaBindings();
	}

	bool GameObject::luaBindingsEnabled()
	{
		return getTag(GTags::LuaBindingsEnabled);
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

	bool GameObject::isRootElement()
	{
		return getTag(GTags::IsRootElement);
	}

	void GameObject::setIsRootElement()
	{
		setTag(GTags::IsRootElement, true);
	}

	void GameObject::markIsDebugElement()
	{
		setTag(GTags::IsDebugElement, true);
	}

	bool GameObject::getIsDebugElement()
	{
		return getTag(GTags::IsDebugElement);
	}

	void GameObject::markIsZone()
	{
		setTag(GTags::IsZone, true);
	}

	bool GameObject::getIsZone()
	{
		return getTag(GTags::IsZone);
	}

	bool GameObject::getHasBoundShaders()
	{
		return getTag(GTags::HasBoundShaders);
	}

	int GameObject::getId()
	{
		return id;
	}

	int GameObject::getBindingId()
	{
		return bindingId;
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
		setTag(GTags::ChildrenSorted, false);

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
			setTag(GTags::ChildrenSorted, false);
		}
	}

	void GameObject::removeFromParent()
	{
		auto parentPointer = parent.lock();
		if (parentPointer != nullptr)
		{
			parentPointer->remove(shared_from_this());
		}
	}

	std::weak_ptr<GameObject> GameObject::getParent()
	{
		return parent;
	}

	const std::list<std::shared_ptr<GameObject>>& GameObject::getChildren()
	{
		return children;
	}

	const std::list<std::shared_ptr<GameObject>>& GameObject::getChildrenOrdered()
	{
		if (!getTag(GTags::ChildrenSorted))
		{
			children.sort(orderLambda);
			setTag(GTags::ChildrenSorted, true);
		}

		return children;
	}

	void GameObject::createChildren()
	{
		if (getTag(GTags::RebuildOnLoad) || !getTag(GTags::HasCreatedChildren))
		{
			onCreateChildren();
			setTag(GTags::HasCreatedChildren, true);
			setTag(GTags::HasHydratedChildren, true);
		}
	}

	void GameObject::destroyChildren()
	{
		onDestroyChildren();
		for (const auto child : children)
		{
			child->detach();
			child->parent = std::weak_ptr<GameObject>();
		}
		children.clear();
		setTag(GTags::ChildrenSorted, false);
		setTag(GTags::HasCreatedChildren, false);
	}

	void GameObject::rebuild()
	{
		destroyChildren();
		createChildren();
		layout();
	}

	void GameObject::registerBoundLuaMethod(const std::string& methodName)
	{
		modules->lua->registerBoundFunction(methodName, sharedPtr());
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

		if (!getTag(GTags::HasBoundShaders))
		{
			onBindShaders();
			setTag(GTags::HasBoundShaders, true);
		}

		if (luaBindingsEnabled() && !getTag(GTags::HasRegisteredLuaBindings))
		{
			onBindLuaHooks();
			setTag(GTags::HasRegisteredLuaBindings, true);
			const auto implKey = serializationClient->getString(luaImplKeyParamName);
			if (currentActiveLuaImpl != implKey && !implKey.empty())
			{
				modules->lua->setObjectImplementation(getLuaBindingId(), implKey);
				currentActiveLuaImpl = implKey;
			}
		}

		setTag(GTags::IsCurrentActive, parentPtr->getTag(GTags::IsCurrentActive) && getTag(GTags::IsCurrentActive));
		if (parentPtr->getInputEnabled() && getInputMode() != InputMode::Disabled)
		{
			setInputMode(InputMode::ParentEnabled);
		}

		if (!getTag(GTags::Loaded) || getTag(GTags::RebuildOnLoad))
		{

			if (!getTag(GTags::DidInitialAttach))
			{
				onInitialAttach();
				setTag(GTags::DidInitialAttach, true);
			}
			else
			{
				if (!children.empty())
				{
					destroyChildren();
				}
			}

			if (getTag(GTags::RebuildOnLoad) || (softAddedChildren.empty() && !getTag(GTags::HasCreatedChildren)))
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
				setTag(GTags::HasHydratedChildren, true);
				layout();
			}

			softAddedChildren.clear();
			setTag(GTags::Loaded, true);
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
		if (getTag(GTags::HasCreatedChildren) && getTag(GTags::HasHydratedChildren))
		{
			onLayoutChildren();
		}
	}

	void GameObject::detach()
	{
		setTag(GTags::IsCurrentActive, false);
		if (getInputMode() != InputMode::Disabled)
		{
			setInputMode(InputMode::Unspecified);
		}

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
				setTag(GTags::ChildrenSorted, false);
			}

			softAddedChildren.clear();
		}

		const auto client = serializationClient->getClient("__application_object__", hint);
		name = client->serializeString("name", name);
		bindingId = client->serializeInt("binding-id", bindingId);
		tags = std::bitset<32>(client->serializeString("tags", tags.to_string()));
		timeScope = (TimeScope)client->serializeInt("timescope", (int)timeScope);
		zIndex = client->serializeInt("z-index", zIndex);

		enterFrameActivated = client->serializeBool("e-f-a", enterFrameActivated);
		enterFramePriority = client->serializeInt("e-f-p", enterFramePriority);
		setInputMode((InputMode)client->serializeInt("i-n-m", (int)getInputMode()));

		switch (hint)
		{
		case SerializationHint::HYDRATE:
		{
			setTag(GTags::IsCurrentActive, false);
			setTag(GTags::DidInitialAttach, false);
			setTag(GTags::HasHydratedChildren, false);
			setTag(GTags::HasBoundShaders, false);
			setTag(GTags::Loaded, false);
			setTag(GTags::HasRegisteredLuaBindings, false);
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
		return !getTag(GTags::RebuildOnLoad);
	}

	bool GameObject::shouldSerializeSelf()
	{
		return getTag(GTags::SerializationEnabled) && !getTag(GTags::IsDebugElement);
	}

	void GameObject::setSerializationEnabled(bool flag)
	{
		setTag(GTags::SerializationEnabled, flag);
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
			modules->event->registerOnEnterFrame(sharedPtr(), priority);
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
		return modules->event->registerTimeoutCallback(sharedPtr(), timeoutMS);
	}

	void GameObject::setTimeout(double timeoutMS, int* timeoutIdLocation)
	{
		if (timeoutIdLocation != 0)
		{
			modules->event->unregisterTimeoutCallback(*timeoutIdLocation);
		}

		*timeoutIdLocation = setTimeout(timeoutMS);
	}

	void GameObject::onEnterFrame(const double& frameTime)
	{
		if (getTag(GTags::LuaBindingsEnabled))
		{
			modules->lua->callBoundFunction(getLuaBindingId(), luaEnterFrameFunctionName, { std::to_string(frameTime) });
		}
	}

	void GameObject::onHandleApplicationEvent(ApplicationEvent* event)
	{
		if (event->stopPropagation) return;

		if (event->direction == EventDirection::Up)
		{
			const auto parentPtr = parent.lock();
			if (parentPtr != nullptr && !parentPtr->isRootElement())
			{
				parentPtr->onHandleApplicationEvent(event);
			}
		}
		else
		{
			for (const auto child : getChildren())
			{
				child->onHandleApplicationEvent(event);

				if (event->stopPropagation) return;
			}
		}
	}

	std::string GameObject::getAwType()
	{
		return schematic == nullptr ? "" : schematic->shortTypeName;
	}

	std::string GameObject::getLuaBindingId()
	{
		return idString;
	}

	void GameObject::onBindLuaHooks()
	{
		modules->lua->registerBoundFunction(luaValidGameObjectFunctionName, shared_from_this());
		modules->lua->registerBoundFunction(luaCreateGameObjectFunctionName, shared_from_this());
		modules->lua->registerBoundFunction(luaSetParentGameObjectFunctionName, shared_from_this());
		modules->lua->registerBoundFunction(luaMoveToParentContainerGameObjectFunctionName, shared_from_this());
		modules->lua->registerBoundFunction(luaGetChildrenGameObjectFunctionName, shared_from_this());

		modules->lua->registerBoundFunction(luaGetParentGameObjectFunctionName, shared_from_this());
		modules->lua->registerBoundFunction(luaFireEventGameObjectFunctionName, shared_from_this());
		modules->lua->registerBoundFunction(luaFireUpEventGameObjectFunctionName, shared_from_this());
	}

	void GameObject::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == luaValidGameObjectFunctionName)
		{
			obj->returnValues.push_back("1");
		}
		else if (func == luaCreateGameObjectFunctionName && obj->args.size() == 2)
		{
			const auto schematic = Hydrater::getShortNameSchematic(StringHelper::toLower(obj->args[0]));
			auto luaImplKey = StringHelper::toLower(obj->args[1]);

			if (schematic != nullptr && modules->lua->hasObjectImplementation(luaImplKey))
			{
				const auto gameObject = std::dynamic_pointer_cast<GameObject>(schematic->factory());
				gameObject->setLuaImplementation(luaImplKey);
				add(gameObject);
				obj->returnValues.push_back(gameObject->getLuaBindingId());
			}
			else
			{
				Logger::instance()->logCritical("GameObject::Failed to create game object for lua with typeName=" + obj->args[0] + ", and luaImpl=" + luaImplKey);
#ifdef _DEBUG
				__debugbreak();
#endif // _DEBUG

			}
		}
		else if (func == luaGetParentGameObjectFunctionName)
		{
			const auto parentPtr = getParent().lock();

			if (parentPtr != nullptr)
			{
				parentPtr->enableLuaBindings();
				obj->returnValues.push_back(parentPtr->getLuaBindingId());
			}
		}
		else if (func == luaGetChildrenGameObjectFunctionName && obj->args.size() == 0)
		{
			for (const auto child : getChildren())
			{
				child->enableLuaBindings();
				obj->returnValues.push_back(child->getLuaBindingId());
			}
		}
		else if (func == luaFireEventGameObjectFunctionName && obj->args.size() == 1) fireEvent(ApplicationEvent(obj->args[0]));
		else if (func == luaFireUpEventGameObjectFunctionName && obj->args.size() == 1) fireUpEvent(obj->args[0]);
		else if (func == luaFireUpEventGameObjectFunctionName && obj->args.size() == 2) fireUpEvent(obj->args[0], obj->args[1]);
		else if (func == luaSetParentGameObjectFunctionName && obj->args.size() == 1)
		{
			const auto newParent = std::dynamic_pointer_cast<GameObject>(modules->lua->getILuaObjectObjectForBindingId(obj->args[0]));
			if (newParent != nullptr)
			{
				const auto self = sharedPtr(); // Grab self ref to prevent destruction during move
				removeFromParent();
				newParent->add(self);
			}
		}
		else if (func == luaMoveToParentContainerGameObjectFunctionName && obj->args.size() == 0)
		{
			const auto parent = getParent().lock();
			if (parent != nullptr)
			{
				const auto parentParent = parent->getParent().lock();
				if (parentParent != nullptr)
				{
					const auto self = sharedPtr(); // Grab self ref to prevent destruction during move

					removeFromParent();
					parentParent->add(self);
				}
			}
		}

	}

	void GameObject::onKey(AWKey code, bool pressed)
	{
		if (getTag(GTags::LuaBindingsEnabled))
		{
			modules->lua->callBoundFunction(getLuaBindingId(), luaOnKeyFunctionName, { std::to_string((int)code), pressed ? "1" : "0" });
		}
	}

	void GameObject::fireEvent(const ApplicationEvent& event)
	{
		modules->event->pushEvent(event);
	}

	void GameObject::fireUpEvent(const std::string& message, std::string data)
	{
		const auto parentPtr = getParent().lock();
		if (parentPtr != nullptr)
		{
			auto e = ApplicationEvent(Events::UpEvent, &data);
			e.message = StringHelper::toLower(message);
			e.direction = EventDirection::Up;
			parentPtr->onHandleApplicationEvent(&e);
		}
	}

	std::shared_ptr<GameObject> GameObject::getRootNode()
	{
		const auto parentPtr = getParent().lock();
		if (parentPtr == nullptr || parentPtr->getTag(GTags::IsRootElement)) return parentPtr;

		return parentPtr->getRootNode();
	}
}
