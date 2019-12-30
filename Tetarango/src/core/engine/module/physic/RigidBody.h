#pragma once

#include "box2d/box2d.h"
#include "util/NumberHelper.h"

namespace AW
{
	class RigidBody
	{
	protected:
		const float scalingFactor = 50.0;
		b2PolygonShape shape;

		b2Body* bodyReference = nullptr;

		float screenToWorldPosition(float pixel);
		float worldToScreenPosition(float world);

		float screenToWorldRotation(float degrees);
		float worldToScreenRotation(float radians);


	public:
		virtual ~RigidBody() = 0 {};

		void onBindBody(b2Body* bodyReference);
		void physicUpdate();
		virtual void onPhysicUpdate() { /* NO-OP */ };

		virtual b2BodyDef onDefineBody() { return b2BodyDef(); };
		virtual b2FixtureDef onDefineFixture() { return b2FixtureDef(); };

		void setMass(float m);
		void setFixedRotation(bool flag);

		void applyForce(float vX, float vY, float amount);
	};
}
