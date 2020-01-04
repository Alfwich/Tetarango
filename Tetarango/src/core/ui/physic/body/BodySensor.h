#pragma once

#include "engine/GameObject.h"
#include "engine/module/physic/RigidBodySensor.h"
#include "ui/physic/body/IBodyListener.h"
#include "box2d/b2_contact.h"
#include "util/Rect.h"

namespace AW
{
	class BodySensor : public GameObject, public RigidBodySensor
	{
		Rect rect;
	public:
		BodySensor();

		void setX(double x);
		double getX();

		void setY(double y);
		double getY();

		void setWidth(double w);
		double getWidth();

		void setHeight(double h);
		double getHeight();

		void onAttach();
		void onDetach();

		virtual b2Fixture* onCreateSensor();
		virtual b2Body* onGetBodyReference();

		std::weak_ptr<IBodyListener> listener;

		virtual void onBeginContact(void* bodyA, void* bodyB, b2Contact* contact);
		virtual void onEndContact(void* bodyA, void* bodyB, b2Contact* contact);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
