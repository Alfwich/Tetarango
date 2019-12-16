#include "Collision.h"

namespace
{
	// Seems like a decent estimate to find non-global colliders - increase if we see lots of issues
	const double collisionSearchRadius = 250.0;
}

namespace AWCore
{
	bool Collision::checkForCollision(const std::shared_ptr<ICollidable>& a, const std::shared_ptr<ICollidable>& b)
	{
		const auto aRect = a->getCollisionRect();
		const auto bRect = b->getCollisionRect();

		if ((aRect->w == 0 && aRect->h == 0) || (bRect->w == 0 && bRect->h == 0))
		{
			return false;
		}

		return aRect->x < bRect->x + bRect->w
			&& aRect->x + aRect->w > bRect->x
			&& aRect->y < bRect->y + bRect->h
			&& aRect->y + aRect->h > bRect->y;
	}

	void Collision::getGlobalColliders(std::vector<std::shared_ptr<ICollidable>>& targets)
	{
		for (auto it = globalColliders.begin(); it != globalColliders.end();)
		{
			const auto globalColliderPtr = (*it)->ptr.lock();

			if (globalColliderPtr != nullptr)
			{
				targets.push_back(globalColliderPtr);
				++it;
			}
			else
			{
				consideredObjectIds.erase((*it)->id);
				it = globalColliders.erase(it);
			}
		}
	}

	void Collision::processCollisions()
	{
		processingCollisions = true;
		auto targets = std::vector<std::shared_ptr<ICollidable>>();
		for (auto it = colliders.begin(); it != colliders.end();)
		{
			const auto colliderPtr = (*it)->ptr.lock();
			const auto ao = std::dynamic_pointer_cast<ApplicationObject>(colliderPtr);

			if (colliderPtr != nullptr)
			{
				if (!colliderPtr->collisionEnabled())
				{
					++it;
					continue;
				}

				const auto cR = colliderPtr->getCollisionRect();

				targets.clear();
				getGlobalColliders(targets);
				collisionQuadMap->find(colliderPtr, collisionSearchRadius, targets);

				for (const auto target : targets)
				{
					if (!target->collisionEnabled())
					{
						continue;
					}

					if (colliderPtr != target && checkForCollision(colliderPtr, target))
					{
						auto shouldFire = false;

						for (const auto myScope : colliderPtr->collisionScopes)
						{
							if (scopeConsiderations.count(myScope) != 0)
							{
								for (const auto targetScope : target->collisionScopes)
								{
									if (targetScope == CollisionScope::Global || scopeConsiderations.at(myScope).count(targetScope) == 1)
									{
										shouldFire = true;
										break;
									}
								}
								break;
							}
						}

						if (!shouldFire)
						{
							continue;
						}

						const auto shouldReconcile = colliderPtr->onCollision(colliderPtr, 0, target, 0);

						if (shouldReconcile && !colliderPtr->onReconcileCollision(colliderPtr, target) && !target->onReconcileCollision(target, colliderPtr))
						{
							Logger::instance()->logCritical("Collision::Failed to reconcile collision");
						}
					}

				}

				++it;
			}
			else
			{
				consideredObjectIds.erase((*it)->id);
				it = colliders.erase(it);
			}
		}

		targets.clear();
		processingCollisions = false;

		while (!cleanupQueue.empty())
		{
			const auto cleanup = cleanupQueue.back();
			unregisterObject(cleanup);
			cleanupQueue.pop_back();
		}
	}

	void Collision::registerScopeConsideration(CollisionScope a, CollisionScope b)
	{
		scopeConsiderations[a].insert(b);
	}

	void Collision::unregisterScopeConsideration(CollisionScope a, CollisionScope b)
	{
		scopeConsiderations[a].erase(b);
	}

	void Collision::registerObjectForCollisionScope(CollisionScope scope, std::shared_ptr<ApplicationObject> obj)
	{
		if (consideredObjectIds.count(obj->getId()) == 1)
		{
			return;
		}

		const auto colliderBundle = std::make_shared<ColliderBundle>(obj);
		if (scope == CollisionScope::Global)
		{
			globalColliders.push_back(colliderBundle);
		}
		else
		{
			colliders.push_back(colliderBundle);
		}

		consideredObjectIds.insert(colliderBundle->id);
	}

	void Collision::unregisterObject(std::shared_ptr<ApplicationObject> obj)
	{
		if (consideredObjectIds.count(obj->getId()) == 0)
		{
			return;
		}

		if (processingCollisions)
		{
			cleanupQueue.push_back(obj);
			return;
		}

		for (auto it = colliders.begin(); it != colliders.end();)
		{
			const auto colliderPtr = (*it)->ptr.lock();
			if (colliderPtr != nullptr)
			{
				if ((*it)->id == obj->getId())
				{
					consideredObjectIds.erase((*it)->id);
					it = colliders.erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
			else
			{
				consideredObjectIds.erase((*it)->id);
				it = colliders.erase(it);
			}
		}

		for (auto it = globalColliders.begin(); it != globalColliders.end();)
		{
			const auto colliderPtr = (*it)->ptr.lock();
			if (colliderPtr != nullptr)
			{
				if ((*it)->id == obj->getId())
				{
					consideredObjectIds.erase((*it)->id);
					it = globalColliders.erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
			else
			{
				consideredObjectIds.erase((*it)->id);
				it = globalColliders.erase(it);
			}
		}
	}

	void Collision::onInit()
	{
		collisionQuadMap = std::make_shared<QuadMap>(-100000, -100000, 500000, 500000);
	}

	void Collision::onCleanup()
	{
		collisionQuadMap->clear();
	}

}
