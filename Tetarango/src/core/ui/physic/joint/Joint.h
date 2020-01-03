#pragma once

#include "engine/GameObject.h"
#include "ui/physic/body/Body.h"

namespace AW
{
	class Joint : public GameObject, public RigidBodyJoint
	{
		int bodyABindingId = 0, bodyBBindingId = 0;
		std::shared_ptr<IBodyListener> findBodyListenerWithBindingId(int id);
	public:
		Joint();

		virtual void onAttach();

		virtual b2Joint* onCreateJoint(const std::shared_ptr<b2World>& world);
		std::weak_ptr<IBodyListener> bodyA, bodyB;

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
