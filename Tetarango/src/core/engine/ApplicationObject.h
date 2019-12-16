#pragma once

#include <string>
#include <vector>
#include <list>
#include <bitset>
#include "ui/renderable/Renderable.h"
#include "engine/module/display/RenderPackage.h"
#include "engine/module/input/IInputListener.h"
#include "engine/module/serialization/ISerializable.h"
#include "engine/module/event/EnterFrameListener.h"
#include "engine/module/collision/ICollidable.h"

namespace AWCore
{
	class SystemModuleBundle;
	class SceneTransitionBundle;

	enum class AOTags {
		IsRootElement = 1,
		IsZone = 2
	};

	class ApplicationObject : public IInputListener, public EnterFrameListener, public ISerializable, public INotifyOnCompletion, public ICollidable, public std::enable_shared_from_this<ApplicationObject>
	{

		bool currentActive = false, active = true, currentInputEnabled = true, didInitialAttach = false, hasCreatedChildren = false, hasHydratedChildren = false, hasClipRect = false;

	protected:
		int id = 0;
		std::bitset<16> tags;
		std::weak_ptr<ApplicationObject> parent;
		std::shared_ptr<Schematic> schematic;
		std::list<std::shared_ptr<ApplicationObject>> children;
		std::list<std::shared_ptr<ISerializable>> softAddedChildren;
		void commandChildren(void(*cmd)(std::shared_ptr<ApplicationObject>));
		TimeScope timeScope = TimeScope::None;

		template <typename T>
		std::shared_ptr<T> findFirstInParentChain();

		template <typename T>
		std::shared_ptr<T> findChildWithName(std::string name, bool checkChildren = true);

		Rect worldRect, screenRect, clipRect;

	public:
		ApplicationObject();

		static int getNextId();

		bool getHasClipRect();
		virtual const Rect* getClipRect();
		virtual void setClipRect(const Rect& rect);
		virtual void setClipRect(const Rect* rect);

		virtual void setWorldRect(Rect* r);
		virtual void updateScreenRect(const RenderPackage* renderPackage);
		virtual Rect* getWorldRect();
		virtual Rect* getScreenRect();

		virtual Rect* getCollisionRect();

		template<typename T>
		void enableSerialization();

		std::string name;
		bool loaded = false, rebuildOnLoad = false, layoutOnLoad = true, visible = true;

		void activate();
		void deactivate();

		bool isActive();

		int zIndex = 0;
		RenderType renderType = RenderType::None;
		RenderPositionMode renderPositionMode = RenderPositionMode::Unspecified;
		RenderPositionMode getFirstNonUnspecifiedRenderPositionMode();

		bool isAttached();

		int getId();
		int inputListenerObjectId() { return getId(); };

		void setTag(AOTags flag, bool value);
		bool getTag(AOTags flag);

		std::shared_ptr<SystemModuleBundle> modules;

		void setTimeScope(TimeScope newScope);
		TimeScope getTimeScope();
		void clearTimeScope();

		void createChildren();
		void destroyChildren();
		void rebuild();

		void attach();
		void layout();
		void detach();

		virtual void enterFrame(double frameTime);

		virtual void onLoadResources() { /* NO-OP */ };
		virtual void onWillTransitioned() { /* NO-OP */ };
		virtual void onTransitionedTo(SceneTransitionBundle& bundle) { /* NO-OP */ };
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

		virtual void add(std::shared_ptr<ApplicationObject> obj);
		virtual void remove(std::shared_ptr<ApplicationObject> obj);
		void removeFromParent();
		std::weak_ptr<ApplicationObject> getParent();
		const std::list<std::shared_ptr<ApplicationObject>>& getChildren();
		const std::list<std::shared_ptr<ApplicationObject>>& getChildrenRenderOrder();

		template <typename T>
		const std::list<std::shared_ptr<T>> getChildrenOfType(bool checkChildren = false);

		virtual std::shared_ptr<Schematic> getSchematic();
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual std::vector<std::shared_ptr<ISerializable>> getSerializableChildren();
		virtual void childHydrated(std::shared_ptr<ISerializable> child);
		virtual bool shouldSerializeChildren();

		virtual bool collisionEnabled();
		virtual void addCollisionScope(CollisionScope scope);
		virtual void removeCollisionScope(CollisionScope scope);

		int getObjectIdEnterFrame();
		virtual void enableEnterFrame(int priority = 0);
		virtual void disableEnterFrame();

		int setTimeout(double timeoutMS);
		void setTimeout(double timeoutMS, int* timeoutIdLocation);
	};

	template<typename T>
	inline std::shared_ptr<T> ApplicationObject::findFirstInParentChain()
	{
		auto parentPtr = parent.lock();

		while (parentPtr != nullptr && !parentPtr->getTag(AOTags::IsRootElement))
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
	inline std::shared_ptr<T> ApplicationObject::findChildWithName(std::string name, bool checkChildren)
	{
		// Check immediate children
		for (const auto child : children)
		{
			const auto castedPtr = std::dynamic_pointer_cast<T>(child);

			if (castedPtr != nullptr && child->name == name)
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
	inline const std::list<std::shared_ptr<T>> ApplicationObject::getChildrenOfType(bool checkChildren)
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
				for (const auto childResult: c1->getChildrenOfType<T>(checkChildren))
				{
					result.push_back(childResult);
				}
			}
		}

		return result;
	}

	template<typename T>
	inline void ApplicationObject::enableSerialization()
	{
		this->typeName = std::string(typeid(T).name());
		if (this->modules->serialization->hasSchematic(this->typeName))
		{
			this->schematic = this->modules->serialization->getSchematic(this->typeName);
			return;
		}

		if (this->modules->status == ModuleBundleStatus::READY || this->modules->status == ModuleBundleStatus::CLEANED_UP)
		{
			this->modules->logger->logFatal("ApplicationObject::Added type=" + this->typeName + " after the application was started. Add this object to it's respective serialization list.");
		}

		this->schematic = std::make_shared<Schematic>(this->typeName, []() -> std::shared_ptr<ISerializable> { return std::make_shared<T>(); });
		this->modules->serialization->registerSchematic(schematic);

		onLoadResources();
	}
}

#include "engine/module/SystemModuleBundle.h"
#include "ui/scene/SceneTransitionBundle.h"
