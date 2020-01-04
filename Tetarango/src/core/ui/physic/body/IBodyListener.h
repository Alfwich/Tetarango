#pragma once

namespace
{
	const auto noop = std::vector<AWVec2<double>>();
}

namespace AW
{
	class Body;

	class IBodyListener
	{
	public:
		virtual std::shared_ptr<Renderable> getRenderableBody() = 0;
		virtual std::shared_ptr<Body> getBodyObject() = 0;

		virtual void onBodyUpdated() { /* NO-OP*/ };
		virtual void onBodyUpdated(int id) { onBodyUpdated(); };

		virtual const std::vector<AWVec2<double>>& getBodyScreenPoints() { return noop; };

		virtual void onBeginContact(const Body* bodyA, const Body* bodyB, b2Contact* contact) { /* NO-OP */ };
		virtual void onEndContact(const Body* bodyA, const Body* bodyB, b2Contact* contact) { /* NO-OP */ };

	};
}

#include "Body.h"

