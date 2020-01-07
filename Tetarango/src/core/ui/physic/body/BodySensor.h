#pragma once

#include "engine/GameObject.h"
#include "engine/module/physic/RigidBodySensor.h"
#include "ui/physic/body/IBodyListener.h"
#include "box2d/b2_contact.h"
#include "IBodyFixture.h"

namespace AW
{
	class BodySensor : public GameObject, public RigidBodySensor, public IBodyFixture
	{
		Rect rect;
		bool hasAttachedSensorCallback = false;

	public:
		BodySensor();

		void setWorldX(float x);
		float getWorldX();

		void setWorldY(float y);
		float getWorldY();

		void setWorldWidth(float w);
		void setWorldHeight(float h);
		void setScreenWidth(float w);
		void setScreenHeight(float h);

		float getWorldWidth() const;
		float getWorldHeight() const;

		void onAttach();
		void onDetach();

		std::weak_ptr<IBodyListener> listener;

		virtual void onBeginContact(b2Body *bodyA, b2Body *bodyB, b2Fixture *fixtureA, b2Fixture *fixtureB);
		virtual void onEndContact(b2Body *bodyA, b2Body *bodyB, b2Fixture *fixtureA, b2Fixture *fixtureB);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		// Inherited via IBodyFixture
		virtual const BodyType getBodyType() const override;
		virtual const AWVec2<float> getBodyWorldSize() override;
		virtual const bool isSensor() const override;
	};
}
