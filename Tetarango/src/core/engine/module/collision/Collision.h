#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "engine/GameObject.h"
#include "engine/module/IBaseModule.h"
#include "ICollidable.h"
#include "util/QuadMap.h"

namespace AW
{

	class Collision : public IBaseModule
	{
		class ColliderBundle
		{
		public:
			ColliderBundle(std::shared_ptr<GameObject>& ao) { this->id = ao->getId(); this->ptr = std::dynamic_pointer_cast<ICollidable>(ao); }
			int id;
			std::weak_ptr<ICollidable> ptr;
		};

		bool processingCollisions = false;

		std::unordered_map<CollisionScope, std::unordered_set<CollisionScope>> scopeConsiderations;
		std::unordered_set<int> consideredObjectIds;
		std::list<std::shared_ptr<ColliderBundle>> colliders;
		std::list<std::shared_ptr<ColliderBundle>> globalColliders;
		std::list<std::shared_ptr<GameObject>> cleanupQueue;

		bool checkForCollision(const std::shared_ptr<ICollidable>& a, const std::shared_ptr<ICollidable>& b);
		void getGlobalColliders(std::vector<std::shared_ptr<ICollidable>>& targets);
		void processCollisions();

	public:
		std::shared_ptr<QuadMap> collisionQuadMap;

		void registerScopeConsideration(CollisionScope consideringScope, CollisionScope targetScope);
		void unregisterScopeConsideration(CollisionScope consideringScope, CollisionScope targetScope);

		void registerObjectForCollisionScope(CollisionScope scope, std::shared_ptr<GameObject> obj);
		void unregisterObject(std::shared_ptr<GameObject> obj);

		void onInit();
		void onEnterFrame();
		void onCleanup();
	};

}

