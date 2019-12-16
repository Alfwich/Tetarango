#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "engine/ApplicationObject.h"
#include "engine/module/IBaseModule.h"
#include "ICollidable.h"
#include "util/QuadMap.h"

namespace AWCore
{

	class Collision : public IBaseModule
	{
		class ColliderBundle
		{
		public:
			ColliderBundle(std::shared_ptr<ApplicationObject>& ao) { this->id = ao->getId(); this->ptr = std::dynamic_pointer_cast<ICollidable>(ao); }
			int id;
			std::weak_ptr<ICollidable> ptr;
		};

		bool processingCollisions = false;

		std::unordered_map<CollisionScope, std::unordered_set<CollisionScope>> scopeConsiderations;
		std::unordered_set<int> consideredObjectIds;
		std::list<std::shared_ptr<ColliderBundle>> colliders;
		std::list<std::shared_ptr<ColliderBundle>> globalColliders;
		std::list<std::shared_ptr<ApplicationObject>> cleanupQueue;

		bool checkForCollision(const std::shared_ptr<ICollidable>& a, const std::shared_ptr<ICollidable>& b);
		void getGlobalColliders(std::vector<std::shared_ptr<ICollidable>>& targets);

	public:
		std::shared_ptr<QuadMap> collisionQuadMap;

		void processCollisions();

		void registerScopeConsideration(CollisionScope consideringScope, CollisionScope targetScope);
		void unregisterScopeConsideration(CollisionScope consideringScope, CollisionScope targetScope);

		void registerObjectForCollisionScope(CollisionScope scope, std::shared_ptr<ApplicationObject> obj);
		void unregisterObject(std::shared_ptr<ApplicationObject> obj);

		void onInit();
		void onCleanup();
	};

}

