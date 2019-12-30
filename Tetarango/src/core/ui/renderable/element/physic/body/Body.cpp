#include "Body.h"

namespace AW
{
	Body::Body()
	{
		fixtureDef.friction = 1.0;
		fixtureDef.density = 1.0;
		registerGameObject<Body>();
	}

	void Body::setX(double x)
	{
		Element::setX(x);

		if (!doingPhysicUpdate && hasBody())
		{
			bodyReference->SetTransform(b2Vec2(screenToWorldPosition(getX()), -screenToWorldPosition(getY())), screenToWorldRotation(getRotation()));
		}
	}

	void Body::setY(double y)
	{
		Element::setY(y);

		if (!doingPhysicUpdate && hasBody())
		{
			bodyReference->SetTransform(b2Vec2(screenToWorldPosition(getX()), -screenToWorldPosition(getY())), screenToWorldRotation(getRotation()));
		}
	}

	void Body::setWidth(double width)
	{
		if (!hasBody())
		{
			Element::setWidth(width);
		}
	}

	void Body::setHeight(double height)
	{
		if (!hasBody())
		{
			Element::setHeight(height);
		}
	}

	void Body::setRotation(double rotation)
	{
		Element::setRotation(rotation);

		if (!doingPhysicUpdate && hasBody())
		{
			bodyReference->SetTransform(b2Vec2(screenToWorldPosition(getX()), -screenToWorldPosition(getY())), screenToWorldRotation(getRotation()));
		}
	}

	void Body::onAttach()
	{
		modules->physic->registerRigidBodyForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBody>(shared_from_this()));
	}

	void Body::onDetach()
	{
		modules->physic->unregisterRigidBodyForWorld(getWorldId(), bodyReference);
	}

	void Body::onBindShaders()
	{
		// NO-OP - Use Default Shaders
	}

	b2Body* AW::Body::onCreateBody(const std::shared_ptr<b2World>& world)
	{
		bodyDef.position.Set(screenToWorldPosition(getX()), -screenToWorldPosition(getY()));
		bodyDef.angle = screenToWorldRotation(getRotation());

		shape.SetAsBox(screenToWorldPosition(getWidth()) / 2.f, screenToWorldPosition(getHeight()) / 2.f);
		fixtureDef.shape = &shape;

		bodyReference = world->CreateBody(&bodyDef);
		bodyReference->CreateFixture(&fixtureDef);

		return bodyReference;
	}

	void AW::Body::onPhysicUpdate()
	{
		const auto pos = bodyReference->GetPosition();
		setPosition(worldToScreenPosition(pos.x), -worldToScreenPosition(pos.y));
		setRotation(worldToScreenRotation(bodyReference->GetAngle()));
	}

}