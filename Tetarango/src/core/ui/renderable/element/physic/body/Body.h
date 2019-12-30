#pragma once

#include "box2d/box2d.h"
#include "ui/renderable/element/Element.h"
#include "engine/module/physic/RigidBody.h"

namespace AW
{
	enum class BodyType
	{
		Box,
		Circle,
		Custom
	};
	class Body : public Element, public RigidBody
	{
		BodyType bodyType = BodyType::Box;

	public:
		Body();

		virtual void setBodyType(BodyType type);

		virtual void setX(double x);
		virtual void setY(double y);
		virtual void setWidth(double width);
		virtual void setHeight(double height);
		virtual void setRotation(double rotation);

		virtual void onAttach();
		virtual void onDetach();
		virtual void onBindShaders();
		virtual b2Body* onCreateBody(const std::shared_ptr<b2World>& world);
		virtual void onPhysicUpdate();
	};
}
