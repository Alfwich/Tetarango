#pragma once

#include "engine/GameObject.h"
#include "ui/physic/body/Body.h"

namespace AW
{
	class Joint : public GameObject, public RigidBodyJoint
	{
		int bodyABindingId = 0, bodyBBindingId = 0;

		void updateBindingReferences(const std::shared_ptr<IBodyListener>& bodyAObj, const std::shared_ptr<IBodyListener>& bodyBObj);
		void ensureBodysAreAttached(const std::shared_ptr<IBodyListener>& bodyA, const std::shared_ptr<IBodyListener>& bodyB);

		std::shared_ptr<IBodyListener> getBodyAObj();
		std::shared_ptr<IBodyListener> getBodyBObj();
			
	public:
		Joint();

		virtual void onAttach();
		virtual void onDetach();

		virtual b2Joint* onCreateJoint(const std::shared_ptr<b2World>& world);
		std::weak_ptr<IBodyListener> bodyA, bodyB;

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
