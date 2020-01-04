#pragma once

#include "box2d/b2_world_callbacks.h"
#include "box2d/b2_contact.h"

namespace AW
{
	class RigidBodySensor : public b2ContactListener
	{
		unsigned int worldId = 0;
		b2Fixture* sensorFixtureReference = nullptr;
		b2Body* bodyReference = nullptr;

	public:

		void BeginContact(b2Contact* contact);
		virtual void onBeginContact(b2Contact* contact) { /* NO-OP*/ };

		void EndContact(b2Contact* contact);
		virtual void onEndContact(b2Contact* contact) { /* NO-OP*/ };

		void createSensor();
		bool hasSensor();
		void onDetach();

		virtual b2Fixture* onCreateSensor() { return nullptr; }
		virtual b2Body* onGetBodyReference() { return nullptr; }

		b2Fixture* getSensor();

		void setWorldId(unsigned int worldId);
		unsigned int getWorldId();
	};
}
