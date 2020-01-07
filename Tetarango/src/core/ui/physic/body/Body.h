#pragma once

#include "box2d/box2d.h"
#include "engine/GameObject.h"
#include "engine/module/physic/RigidBody.h"
#include "IBodyListener.h"
#include "IBodyFixture.h"

namespace AW
{

	class Body : public GameObject, public RigidBody
	{
		bool autoUpdate = true;

		void notifyListenerOnPhysicUpdate();

		std::shared_ptr<IBodyFixture> getBodyFixtureFromListener();
		std::shared_ptr<Renderable> getRenderTargetFromListener();

		void createFixture(const std::shared_ptr<IBodyFixture>& fixture);

	public:
		Body();

		void setAutoUpdate(bool flag);

		std::weak_ptr<IBodyListener> listener;

		virtual void updateBodyForRenderable();

		virtual void onAttach();
		virtual void onDetach();

		virtual void applyForce(float vX, float vY, float amount) override;
		virtual void applyForce(float vX, float vY, float cX, float cY, float amount) override;

		void applyImpulse(float vX, float vY, float amount);
		void applyImpulse(float vX, float vY, float cX, float cY, float amount);

		virtual void add(std::shared_ptr<GameObject> obj) override;
		virtual void remove(std::shared_ptr<GameObject> obj) override;

		virtual b2Body* onCreateBody(const std::shared_ptr<b2World>& world);
		virtual void onPhysicUpdate();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

	};
}
