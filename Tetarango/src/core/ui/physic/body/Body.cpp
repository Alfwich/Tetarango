#include "Body.h"

#include "ui/renderable/primitive/Polygon.h"

namespace AW
{
	Body::Body()
	{
		fixtureDef.friction = 1.0;
		fixtureDef.density = 1.0;
		GORegister(Body);
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

	void Body::applyForce(float vX, float vY, float amount)
	{
		RigidBody::applyForce(vX, vY, amount * (modules->physic->getPhysicFrameDeltaTime() / 1000.0));
	}

	void Body::applyForce(float vX, float vY, float cX, float cY, float amount)
	{
		RigidBody::applyForce(vX, vY, cX, cY, amount * (modules->physic->getPhysicFrameDeltaTime() / 1000.0));
	}

	b2Body* Body::onCreateBody(const std::shared_ptr<b2World>& world)
	{
		const auto rend = getRenderableFromListener();
		if (rend == nullptr)
		{
			return nullptr;
		}

		bodyDef.position.Set(rend->getWorldX(), rend->getWorldY());
		bodyDef.angle = rend->getWorldRotation();

		bodyReference = world->CreateBody(&bodyDef);
		switch (bodyType)
		{
		case BodyType::Box:
		{
			b2PolygonShape shape;

			shape.SetAsBox(screenToWorldPosition(rend->getScreenWidth()) / 2.f, screenToWorldPosition(rend->getScreenHeight()) / 2.f);
			shape.SetAsBox(rend->getWorldWidth() / 2.f, rend->getWorldHeight() / 2.f);
			fixtureDef.shape = &shape;

			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Circle:
		{
			b2CircleShape shape;

			shape.m_radius = std::max(rend->getWorldWidth(), rend->getWorldHeight()) / 2.f;
			fixtureDef.shape = &shape;

			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Polygon:
		{
			const auto listenerPtr = listener.lock();
			if (listenerPtr != nullptr)
			{
				b2PolygonShape shape;

				const auto screenPoints = listenerPtr->getBodyWorldPoints();
				std::vector<b2Vec2> pts;
				for (const auto& sp : screenPoints) pts.push_back(b2Vec2(sp.x, sp.y));

				shape.Set(&pts[0], (unsigned int)pts.size());
				fixtureDef.shape = &shape;

				bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		case BodyType::Line:
		{
			const auto listenerPtr = listener.lock();
			if (listenerPtr != nullptr)
			{
				b2EdgeShape shape;

				const auto xOffset = rend->getWorldWidth() / 2.f;
				const auto yOffset = rend->getWorldHeight() / 2.f;

				b2Vec2 p1{ -xOffset, -yOffset }, p2{ xOffset, -yOffset };
				shape.Set(p1, p2);
				fixtureDef.shape = &shape;

				bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		case BodyType::Chain:
		{
			const auto listenerPtr = listener.lock();
			if (listenerPtr != nullptr)
			{
				b2ChainShape shape;

				const auto screenPoints = listenerPtr->getBodyWorldPoints();
				std::vector<b2Vec2> pts;
				for (const auto& sp : screenPoints) pts.push_back(b2Vec2(sp.x, sp.y));

				shape.CreateChain(&pts[0], (unsigned int)pts.size());
				fixtureDef.shape = &shape;

				bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		}

		bodyReference->SetUserData(this);

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
				rend->setWorldPosition(pos.x, pos.y);
				rend->setWorldRotation(bodyReference->GetAngle());
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

		const auto pos = b2Vec2(rend->getWorldX(), rend->getWorldY());
		const auto rotation = rend->getWorldRotation();

		bodyReference->SetTransform(pos, rotation);
	}

}