#pragma once

#include "box2d/box2d.h"
#include "engine/GameObject.h"
#include "engine/module/physic/RigidBody.h"
#include "IBodyListener.h"

namespace AW
{
	enum class BodyType
	{
		Box,
		Circle,
		Polygon,
		Line,
		Chain
	};

	class Body : public GameObject, public RigidBody
	{
		BodyType bodyType = BodyType::Box;
		bool autoUpdate = true;
		AWVec2<float> size;

		void notifyListenerOnPhysicUpdate();

		std::shared_ptr<Renderable> getShapeFromListener();
		std::shared_ptr<Renderable> getRenderTargetFromListener();

	public:
		Body();

		void setAutoUpdate(bool flag);
		void setBodyType(BodyType type);

		std::weak_ptr<IBodyListener> listener;

		virtual void updateBodyForRenderable();

		virtual void onAttach();
		virtual void onDetach();

		virtual void setWorldWidth(float width);
		virtual void setWorldHeight(float height);
		virtual void setScreenWidth(float width);
		virtual void setScreenHeight(float height);

		virtual float getWorldWidth();
		virtual float getWorldHeight();

		virtual void applyForce(float vX, float vY, float amount) override;
		virtual void applyForce(float vX, float vY, float cX, float cY, float amount) override;

		void applyImpulse(float vX, float vY, float amount);
		void applyImpulse(float vX, float vY, float cX, float cY, float amount);

		virtual b2Body* onCreateBody(const std::shared_ptr<b2World>& world);
		virtual void onPhysicUpdate();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

	};
}
