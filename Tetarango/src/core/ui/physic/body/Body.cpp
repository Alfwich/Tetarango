#include "Body.h"

#include "ui/renderable/primitive/Polygon.h"

namespace AW
{
	Body::Body()
	{
		fixtureDef.friction = 1.0;
		fixtureDef.density = 1.0;
		registerGameObject<Body>(__FUNCTION__);
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
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			return listenerPtr->getRenderableBody();
		}

		return nullptr;
	}

	std::vector<b2Vec2> Body::translateScreenPointsToWorldPoints(const std::shared_ptr<Renderable>& rend, const std::vector<AWVec2<double>>& screenPoints)
	{
		const auto widthFactor = screenToWorldPosition(rend->getWidth());
		const auto heightFactor = screenToWorldPosition(rend->getHeight());
		const auto translate = AWVec2<float>(-widthFactor / 2.0, heightFactor / 2.0);
		const auto scaleFactor = AWVec2<double>(1.0 / widthFactor, 1.0 / heightFactor);
		auto b2Pts = std::vector<b2Vec2>();

		for (const auto pt : screenPoints)
		{
			auto world = screenToWorld(pt).add(translate);
			b2Pts.push_back(world.asB2());
		}

		return b2Pts;
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

	std::shared_ptr<SerializationClient> Body::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__body__", hint);

		bodyType = (BodyType)client->serializeInt("body-type", (int)bodyType);

		RigidBody::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
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
		case BodyType::Box:
		{
			auto shape = b2PolygonShape();
			shape.SetAsBox(screenToWorldPosition(rend->getWidth()) / 2.f, screenToWorldPosition(rend->getHeight()) / 2.f);
			fixtureDef.shape = &shape;
			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Circle:
		{
			auto shape = b2CircleShape();
			shape.m_radius = screenToWorldPosition((float)std::max(rend->getWidth(), rend->getHeight()) / 2.f);
			fixtureDef.shape = &shape;
			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Polygon:
		{
			const auto listenerPtr = listener.lock();
			if (listenerPtr != nullptr)
			{
				auto shape = b2PolygonShape();
				const auto screenPoints = translateScreenPointsToWorldPoints(rend, listenerPtr->getBodyScreenPoints());

				shape.Set(&screenPoints[0], (unsigned int)screenPoints.size());
				fixtureDef.shape = &shape;
				bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		case BodyType::Custom:
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

	void Body::updateBodyForRenderable()
	{
		const auto rend = getRenderableFromListener();
		if (rend == nullptr)
		{
			return;
		}

		const auto pos = b2Vec2(screenToWorldPosition(rend->getX()), -screenToWorldPosition(rend->getY()));
		const auto rotation = screenToWorldRotation(rend->getRotation());
		bodyReference->SetTransform(pos, rotation);
	}

}