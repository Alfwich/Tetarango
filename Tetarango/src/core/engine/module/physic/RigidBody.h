#pragma once

#include "box2d/box2d.h"
#include "util/NumberHelper.h"

namespace AW
{
	class RigidBody
	{
	protected:
		const float scalingFactor = 10.0;
		b2PolygonShape shape;

		float screenToWorldPosition(double pixel) { return (float)pixel / scalingFactor; };
		double worldToScreenPosition(float world) { return (double)world * (float)scalingFactor; };

		float screenToWorldRotation(double degrees) { return (float)degrees * -(float)(AW::NumberHelper::PI / 180.0); };
		double worldToScreenRotation(float radians) { return (double)radians * -(180.0 / AW::NumberHelper::PI); };


	public:
		virtual ~RigidBody() = 0 {};
		virtual void onPhysicUpdate(const b2Body* body) { /* NO-OP */ };

		virtual b2BodyDef onDefineBody() { return b2BodyDef(); };
		virtual b2FixtureDef onDefineFixture() { return b2FixtureDef(); };
	};
}
