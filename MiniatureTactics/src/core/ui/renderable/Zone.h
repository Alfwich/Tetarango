#pragma once

#include "ui/renderable/container/Container.h"

namespace MT
{

	class Zone : public Container
	{
		bool requestReconciliation;

	public:
		Zone();

		void visualize();
		void setRequestReconciliation(bool flag);

		std::weak_ptr<ICollidable> notifyOnCollision;
		std::weak_ptr<ICollidable> notifyOnReconcileCollision;

		virtual void onInitialAttach();
		virtual bool onCollision(std::shared_ptr<ICollidable> you, int yourScope, std::weak_ptr<ICollidable> other, int otherScope);
		virtual bool onReconcileCollision(std::shared_ptr<ICollidable> you, std::shared_ptr<ICollidable> other);
	};

}
