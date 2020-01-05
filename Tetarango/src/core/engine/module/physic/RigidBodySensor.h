#pragma once

#include "box2d/b2_fixture.h"
#include "box2d/b2_body.h"

namespace AW
{
	class RigidBodySensor
	{
		unsigned int worldId = 0;
		b2Fixture* sensorFixtureReference = nullptr;
		b2Body* bodyReference = nullptr;

	public:

		void beginContact(b2Fixture* fixtureA, b2Fixture* fixtureB);
		virtual void onBeginContact(b2Body *bodyA, b2Body *bodyB, b2Fixture *fixtureA, b2Fixture *fixtureB) { /* NO-OP*/ };

		void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
		virtual void onEndContact(b2Body *bodyA, b2Body *bodyB, b2Fixture *fixtureA, b2Fixture *fixtureB) { /* NO-OP*/ };

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
