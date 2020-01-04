#pragma once

#include "engine/GameObject.h"
#include "engine/module/physic/RigidBodySensor.h"
#include "ui/physic/body/IBodyListener.h"
#include "box2d/b2_contact.h"

namespace AW
{
	class BodySensor : public GameObject, public RigidBodySensor
	{
	public:
		BodySensor();
		double x = 0, y = 0, width = 0, height = 0;

		void onAttach();
		void onDetach();

		virtual b2Fixture* onCreateSensor();
		virtual b2Body* onGetBodyReference();

		std::weak_ptr<IBodyListener> listener;

		virtual void onBeginContact(b2Contact* contact);
		virtual void onEndContact(b2Contact* contact);

	};
}
