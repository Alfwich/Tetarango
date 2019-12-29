#pragma once

#include "box2d/box2d.h"

namespace AW
{
	class RigidBody
	{
	protected:
			b2PolygonShape shape;

	public:
		virtual ~RigidBody() = 0 {};
		virtual void onPhysicUpdate(const b2Body* body) { /* NO-OP */ };

		virtual b2BodyDef onDefineBody() { return b2BodyDef(); };
		virtual b2FixtureDef onDefineFixture() { return b2FixtureDef(); };
	};
}
