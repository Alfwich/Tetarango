#include "Body.h"

namespace AW
{
	Body::Body()
	{
		fixtureDef.friction = 1.0;
		fixtureDef.density = 1.0;
		registerGameObject<Body>();
	}

	void Body::setBodyType(BodyType type)
	{
		bodyType = type;
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
		if (!hasBody())
		{
			modules->physic->registerRigidBodyForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBody>(shared_from_this()));
		}
	}

	void Body::onDetach()
	{
		if (hasBody())
		{
			modules->physic->unregisterRigidBodyForWorld(getWorldId(), bodyReference);
		}
	}

	void Body::onBindShaders()
	{
		// NO-OP - Use Default Shaders
	}

	b2Body* AW::Body::onCreateBody(const std::shared_ptr<b2World>& world)
	{
		bodyDef.position.Set(screenToWorldPosition(getX()), -screenToWorldPosition(getY()));
		bodyDef.angle = screenToWorldRotation(getRotation());

		bodyReference = world->CreateBody(&bodyDef);
		switch (bodyType)
		{
		case AW::BodyType::Box:
		{
			auto shape = b2PolygonShape();
			shape.SetAsBox(screenToWorldPosition(getWidth()) / 2.f, screenToWorldPosition(getHeight()) / 2.f);
			fixtureDef.shape = &shape;
			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case AW::BodyType::Circle:
		{
			auto shape = b2CircleShape();
			shape.m_radius = screenToWorldPosition((float)std::max(getWidth(), getHeight()) / 2.f);
			fixtureDef.shape = &shape;
			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case AW::BodyType::Custom:
		default:
			// Do nothing - expect user to create fixtures
			break;
		}


		return bodyReference;
	}

	void AW::Body::onPhysicUpdate()
	{
		const auto pos = bodyReference->GetPosition();
		setPosition(worldToScreenPosition(pos.x), -worldToScreenPosition(pos.y));
		setRotation(worldToScreenRotation(bodyReference->GetAngle()));
	}

}