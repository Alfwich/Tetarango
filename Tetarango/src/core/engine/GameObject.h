#pragma once

#include "engine/module/ModuleBundleStatus.h"
#include "ui/renderable/Renderable.h"
#include "engine/module/display/RenderPackage.h"
#include "engine/module/input/IInputListener.h"
#include "engine/module/serialization/ISerializable.h"
#include "engine/module/event/EnterFrameListener.h"
#include "engine/module/physic/RigidBody.h"

#define GORegister(x) registerGameObject<x>(__FUNCTION__);

namespace AW
{
	class SystemModuleBundle;
	class SceneTransitionBundle;

	enum class GTags
	{
		IsRootElement,
		IsZone,
		IsDebugElement,

		Loaded,
		RebuildOnLoad,
		LayoutOnLoad,
		SerializationEnabled,
		ChildrenSorted,

		IsActive,
		IsCurrentActive,
		DidInitialAttach,
		HasCreatedChildren,
		HasHydratedChildren,
		HasClipRect,
		HasBoundShaders
	};

	class GameObject : public IInputListener, public EnterFrameListener, public ISerializable, public INotifyOnCompletion, public ILuaObject, public std::enable_shared_from_this<GameObject>
	{
		const int id;
		int bindingId;

		std::bitset<32> tags;

		bool getTag(GTags tag);
		void setTag(GTags tag, bool value);

		std::list<std::shared_ptr<GameObject>> children;

	protected:
		std::weak_ptr<GameObject> parent;
		std::shared_ptr<Schematic> schematic;
		std::list<std::shared_ptr<ISerializable>> softAddedChildren;
		void commandChildren(void(*cmd)(std::shared_ptr<GameObject>));
		TimeScope timeScope = TimeScope::None;

		template<typename T>
		void registerGameObject(const std::string& typeName);

	public:
		template <typename T>
		std::shared_ptr<T> findFirstInParentChain();

		template <typename T>
		std::shared_ptr<T> findChildWithName(std::string name, bool checkChildren = true);

		template <typename T>
		std::shared_ptr<T> findChildWithBindingId(int bindingId);

		template <typename T>
		std::shared_ptr<T> findChildWithBindingIdWholeGraph(int bindingId);

		std::shared_ptr<GameObject> getRootNode();

		template <typename T>
		std::weak_ptr<T> weakPtr();

		template <typename T>
		std::shared_ptr<T> sharedPtr();

		std::weak_ptr<GameObject> weakPtr() { return weak_from_this(); };
		std::shared_ptr<GameObject> sharedPtr() { return shared_from_this(); };

		static int nextId();
		static int currentBindingId();

		GameObject();

		int getId();
		virtual int getBindingId();

		std::string name;
		int zIndex = 0;

		void activate();
		void deactivate();

		bool isActive();
		bool getHasBoundShaders();

		void setShouldRebuildOnLoad(bool flag = true);
		bool shouldRebuildOnLoad();

		void setShouldLayoutOnLoad(bool flag = true);
		bool shouldLayoutOnLoad();

		bool isAttached();
		bool isRootElement();
		void setIsRootElement();

		void markIsDebugElement();
		bool getIsDebugElement();

		void markIsZone();
		bool getIsZone();

		int inputListenerObjectId() { return getId(); };

		const std::shared_ptr<SystemModuleBundle> modules;

		void setTimeScope(TimeScope newScope);
		TimeScope getTimeScope();
		void clearTimeScope();

		void createChildren();
		void destroyChildren();
		void rebuild();

		void attach();
		void layout();
		void detach();

		virtual void enterFrame(const double& frameTime);

		virtual void onLoadResources() { /* NO-OP */ };
		virtual void onWillTransitioned() { /* NO-OP */ };
		virtual void onTransitionedTo() { /* NO-OP */ };
		virtual void onTransitionedTo(SceneTransitionBundle& bundle) { onTransitionedTo(); };
		virtual void onBindShaders() { /* NO-OP */ };
		virtual void onInitialAttach() { /* NO-OP */ };
		virtual void onCreateChildren() { /* NO-OP */ };
		virtual void onLayoutChildren() { /* NO-OP */ };
		virtual void onDestroyChildren() { /* NO-OP */ };
		virtual void onChildrenHydrated() { /* NO-OP */ };
		virtual void onAttach() { /* NO-OP */ };
		virtual void onDetach() { /* NO-OP */ };
		virtual void onDisplayProvisioned() { /* NO-OP */ };
		virtual void onTimeScopeChanged() { /* NO-OP */ };
		virtual void onWorkError(WORKER_ID workerId, WorkerTaskCode code) { /* NO-OP */ };
		virtual void onWorkDone(WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<AsyncResultBundle> result) { /* NO-OP */ };

		virtual void add(std::shared_ptr<GameObject> obj);
		virtual void remove(std::shared_ptr<GameObject> obj);
		void removeFromParent();
		std::weak_ptr<GameObject> getParent();
		const std::list<std::shared_ptr<GameObject>>& getChildren();
		const std::list<std::shared_ptr<GameObject>>& getChildrenOrdered();

		template <typename T>
		const std::list<std::shared_ptr<T>> getChildrenOfType(bool checkChildren = false);

		virtual std::shared_ptr<Schematic> getSchematic();
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual std::vector<std::shared_ptr<ISerializable>> getSerializableChildren();
		virtual void childHydrated(std::shared_ptr<ISerializable> child);
		virtual bool shouldSerializeChildren();
		virtual bool shouldSerializeSelf();
		void setSerializationEnabled(bool flag = true);

		int getObjectIdEnterFrame();
		virtual void enableEnterFrame(int priority = 0);
		virtual void disableEnterFrame();

		int setTimeout(double timeoutMS);
		void setTimeout(double timeoutMS, int* timeoutIdLocation);

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override;
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
};

	template<typename T>
	inline std::shared_ptr<T> GameObject::findFirstInParentChain()
	{
		auto parentPtr = parent.lock();

		while (parentPtr != nullptr && !parentPtr->getTag(GTags::IsRootElement))
		{
			const auto castedParent = std::dynamic_pointer_cast<T>(parentPtr);

			if (castedParent != nullptr)
			{
				return castedParent;
			}

			parentPtr = parentPtr->parent.lock();
		}

		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::findChildWithName(std::string name, bool checkChildren)
	{
		// Check immediate children
		std::shared_ptr<T> castedPtr;
		for (const auto child : children)
		{
			if (child->name == name && (castedPtr = std::dynamic_pointer_cast<T>(child)) != nullptr)
			{
				return castedPtr;
			}
		}

		if (checkChildren)
		{
			// Check children recursive
			for (const auto child : children)
			{
				for (const auto childChild : child->getChildren())
				{
					const auto result = child->findChildWithName<T>(name, checkChildren);

					if (result != nullptr)
					{
						return result;
					}
				}
			}
		}

		// No match
		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::findChildWithBindingId(int bindingId)
	{
		// Check immediate children
		for (const auto child : children)
		{
			if (child->getBindingId() == bindingId) return std::dynamic_pointer_cast<T>(child);
		}

		// Check children recursive
		for (const auto child : children)
		{
			const auto obj = child->findChildWithBindingId<T>(bindingId);
			if (obj != nullptr) return obj;
		}

		// No match
		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::findChildWithBindingIdWholeGraph(int bindingId)
	{
		const auto root = getRootNode();
		if (root != nullptr)
		{
			return root->findChildWithBindingId(bindingId);
		}
	}

	template<typename T>
	inline std::weak_ptr<T> GameObject::weakPtr()
	{
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::sharedPtr()
	{
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	template<typename T>
	inline const std::list<std::shared_ptr<T>> GameObject::getChildrenOfType(bool checkChildren)
	{
		std::list<std::shared_ptr<T>> result;

		for (const auto c : children)
		{
			const auto typedPtr = std::dynamic_pointer_cast<T>(c);

			if (typedPtr != nullptr)
			{
				result.push_back(typedPtr);
			}
		}

		if (checkChildren)
		{
			for (const auto c1 : children)
			{
				for (const auto childResult : c1->getChildrenOfType<T>(checkChildren))
				{
					result.push_back(childResult);
				}
			}
		}

		return result;
	}

	template<typename T>
	inline void GameObject::registerGameObject(const std::string& typeName)
	{
		this->typeName = typeName;
		if (this->modules->serialization->hasSchematic(this->typeName))
		{
			this->schematic = this->modules->serialization->getSchematic(this->typeName);
			return;
		}

		if (this->modules->status == ModuleBundleStatus::READY || this->modules->status == ModuleBundleStatus::CLEANED_UP)
		{
			this->modules->logger->logFatal("GameObject::Added type=" + this->typeName + " after the application was started. Call GORegister(T); in the ctor");
		}

		this->schematic = std::make_shared<Schematic>(this->typeName, []() -> std::shared_ptr<ISerializable> { return std::make_shared<T>(); });
		this->modules->serialization->registerSchematic(schematic);
	}
}

#include "engine/module/SystemModuleBundle.h"
#include "ui/scene/SceneTransitionBundle.h"
