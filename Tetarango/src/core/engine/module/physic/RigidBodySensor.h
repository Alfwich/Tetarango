#pragma once

#include "box2d/b2_fixture.h"
#include "box2d/b2_body.h"

namespace AW
{
	class RigidBodySensor
	{
		unsigned int worldId = 0;

	public:

		void beginContact(b2Fixture* fixtureA, b2Fixture* fixtureB);
		virtual void onBeginContact(b2Body* bodyA, b2Body* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) { /* NO-OP*/ };

		void endContact(b2Fixture* fixtureA, b2Fixture* fixtureB);
		virtual void onEndContact(b2Body* bodyA, b2Body* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) { /* NO-OP*/ };

		void setWorldId(unsigned int worldId);
		unsigned int getWorldId();
	};
}
