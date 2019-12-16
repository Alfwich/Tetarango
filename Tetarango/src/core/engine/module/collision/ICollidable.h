#pragma once

#include <set>

namespace AWCore
{
	enum class CollisionScope
	{
		Global,
		Player,
		General,
		Transfer,
		Projectile,
		Item
	};

	class ICollidable
	{
	public:
		std::set<CollisionScope> collisionScopes;
		virtual bool collisionEnabled() = 0;
		virtual Rect* getCollisionRect() { return nullptr; }
		virtual bool onCollision(std::shared_ptr<ICollidable> you, int yourScope, std::weak_ptr<ICollidable> other, int otherScope) { return false; };
		virtual bool onReconcileCollision(std::shared_ptr<ICollidable> you, std::shared_ptr<ICollidable> other) { return false; };

		bool operator<(const ICollidable& rhs) const
		{
			return false;
		}
	};
}
