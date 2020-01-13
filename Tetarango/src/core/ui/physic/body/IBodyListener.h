#pragma once

namespace AW
{
	class Body;

	class ContactBundle
	{
	public:
		ContactBundle(void* bodyA, void* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB)
			: bodyA(static_cast<Body*>(bodyA)), bodyB(static_cast<Body*>(bodyB)), fixtureA(fixtureA), fixtureB(fixtureB)
		{};

		Body* bodyA, * bodyB;
		b2Fixture* fixtureA, * fixtureB;
	};

	class IBodyListener
	{
	public:
		virtual std::shared_ptr<Renderable> getRenderableBody() = 0;
		virtual std::shared_ptr<Body> getBodyObject() = 0;

		virtual void onBodyUpdated() { /* NO-OP*/ };
		virtual void onBodyUpdated(int id) { onBodyUpdated(); };

		virtual void onBeginContact(std::unique_ptr<ContactBundle> bundle) { /* NO-OP */ };
		virtual void onEndContact(std::unique_ptr<ContactBundle> bundle) { /* NO-OP */ };

	};
}

#include "Body.h"

