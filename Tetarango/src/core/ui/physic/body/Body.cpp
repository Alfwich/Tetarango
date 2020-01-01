#include "Body.h"

namespace AW
{

	Body::Body()
	{
		fixtureDef.friction = 1.0;
		fixtureDef.density = 1.0;
		registerGameObject<Body>("p-body");
	}

	void Body::notifyListenerOnPhysicUpdate()
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onBodyUpdated(getId());
		}
	}

	std::shared_ptr<Renderable> Body::getRenderableFromListener()
	{
		return std::dynamic_pointer_cast<Renderable>(listener.lock());
	}

	void Body::setAutoUpdate(bool flag)
	{
		autoUpdate = flag;
	}

	void Body::setBodyType(BodyType type)
	{
		bodyType = type;
	}

	void Body::onAttach()
	{
		if (listener.lock() == nullptr)
		{
			listener = std::dynamic_pointer_cast<IBodyListener>(getParent().lock());
		}

		if (listener.lock() == nullptr)
		{
			Logger::instance()->logCritical("Body::Failed to bind listener on attach for Body");
			return;
		}

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

	b2Body* Body::onCreateBody(const std::shared_ptr<b2World>& world)
	{
		const auto rend = getRenderableFromListener();
		if (rend == nullptr)
		{
			return nullptr;
		}

		bodyDef.position.Set(screenToWorldPosition(rend->getX()), -screenToWorldPosition(rend->getY()));
		bodyDef.angle = screenToWorldRotation(rend->getRotation());

		bodyReference = world->CreateBody(&bodyDef);
		switch (bodyType)
		{
		case AW::BodyType::Box:
		{
			auto shape = b2PolygonShape();
			shape.SetAsBox(screenToWorldPosition(rend->getWidth()) / 2.f, screenToWorldPosition(rend->getHeight()) / 2.f);
			fixtureDef.shape = &shape;
			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case AW::BodyType::Circle:
		{
			auto shape = b2CircleShape();
			shape.m_radius = screenToWorldPosition((float)std::max(rend->getWidth(), rend->getHeight()) / 2.f);
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

	void Body::onPhysicUpdate()
	{
		if (autoUpdate)
		{
			const auto rend = getRenderableFromListener();
			if (rend != nullptr)
			{
				const auto pos = bodyReference->GetPosition();
				rend->setPosition(worldToScreenPosition(pos.x), -worldToScreenPosition(pos.y));
				rend->setRotation(worldToScreenRotation(bodyReference->GetAngle()));
			}
		}
		else
		{
			notifyListenerOnPhysicUpdate();
		}
	}

	void Body::updateBodyForRenderable(const std::shared_ptr<Renderable>& rend)
	{
		const auto pos = b2Vec2(screenToWorldPosition(rend->getX()), -screenToWorldPosition(rend->getY()));
		const auto rotation = screenToWorldRotation(rend->getRotation());
		bodyReference->SetTransform(pos, rotation);
	}

}