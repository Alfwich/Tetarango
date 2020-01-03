#pragma once

#include "engine/GameObject.h"
#include "ui/physic/body/Body.h"

namespace AW
{
	enum class JointType
	{
		Distant,
		Revolute
	};

	class Joint : public GameObject, public RigidBodyJoint
	{
		int bodyABindingId = 0, bodyBBindingId = 0;
		float jointDistance = 0.f;

		JointType jointType = JointType::Distant;

		void updateBindingReferences(const std::shared_ptr<IBodyListener>& bodyAObj, const std::shared_ptr<IBodyListener>& bodyBObj);
		void ensureBodysAreAttached(const std::shared_ptr<IBodyListener>& bodyA, const std::shared_ptr<IBodyListener>& bodyB);

		std::shared_ptr<IBodyListener> getBodyAListener();
		std::shared_ptr<IBodyListener> getBodyBListener();

		std::shared_ptr<Body> getBodyA();
		std::shared_ptr<Body> getBodyB();

	public:
		Joint();

		void setJointType(JointType type);

		virtual void onAttach();
		virtual void onDetach();

		virtual b2Joint* onCreateJoint(const std::shared_ptr<b2World>& world);
		std::weak_ptr<IBodyListener> bodyA, bodyB;

		void setJointDistance(float distance);
		double getJointDistance();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		// Inherited via RigidBodyJoint
		virtual bool hasValidBodyReferences() override;
	};
}
