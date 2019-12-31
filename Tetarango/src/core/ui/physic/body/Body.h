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
		Custom
	};
	class Body : public GameObject, public RigidBody
	{
		BodyType bodyType = BodyType::Box;
		bool autoUpdate = true;

		void notifyListenerOnPhysicUpdate();
		std::shared_ptr<Renderable> getRenderableFromListener();

	public:
		Body();

		void setAutoUpdate(bool flag);
		void setBodyType(BodyType type);

		std::weak_ptr<IBodyListener> listener;
		virtual void updateBodyForRenderable(const std::shared_ptr<Renderable>& rend);

		virtual void onAttach();
		virtual void onDetach();

		virtual b2Body* onCreateBody(const std::shared_ptr<b2World>& world);
		virtual void onPhysicUpdate();

	};
}
