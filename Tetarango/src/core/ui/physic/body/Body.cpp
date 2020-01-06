#include "Body.h"

#include "ui/renderable/primitive/Polygon.h"
#include "ui/renderable/Renderable.h"

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

	std::shared_ptr<Renderable> Body::getShapeFromListener()
	{
		const auto lPtr = std::dynamic_pointer_cast<IBodyListener>(listener.lock());
		if (lPtr != nullptr) return lPtr->getShape();

		return nullptr;
	}

	std::shared_ptr<Renderable> Body::getRenderTargetFromListener()
	{
		const auto lPtr = std::dynamic_pointer_cast<IBodyListener>(listener.lock());
		if (lPtr != nullptr) return lPtr->getRenderableBody();

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

	void Body::setWorldWidth(float width)
	{
		size.x = width;
	}

	void Body::setWorldHeight(float height)
	{
		size.y = height;
	}

	void Body::setScreenWidth(float width)
	{
		size.x = Renderable::screenToWorldPosition(width);
	}

	void Body::setScreenHeight(float height)
	{
		size.y = Renderable::screenToWorldPosition(height);
	}

	float Body::getWorldWidth()
	{
		return 0.0f;
	}

	float Body::getWorldHeight()
	{
		return 0.0f;
	}

	void Body::applyForce(float vX, float vY, float amount)
	{
		RigidBody::applyForce(vX, vY, amount * (modules->physic->getPhysicFrameDeltaTime() / 1000.0));
	}

	void Body::applyForce(float vX, float vY, float cX, float cY, float amount)
	{
		RigidBody::applyForce(vX, vY, cX, cY, amount * (modules->physic->getPhysicFrameDeltaTime() / 1000.0));
	}

	void Body::applyImpulse(float vX, float vY, float amount)
	{
		RigidBody::applyForce(vX, vY, amount);
	}

	void Body::applyImpulse(float vX, float vY, float cX, float cY, float amount)
	{
		RigidBody::applyForce(vX, vY, cX, cY, amount);
	}

	b2Body* Body::onCreateBody(const std::shared_ptr<b2World>& world)
	{
		const auto rend = getShapeFromListener();
		if (rend == nullptr)
		{
			return nullptr;
		}

		if (size.x == 0 && size.y == 0)
		{
			size.x = rend->getWorldWidth();
			size.y = rend->getWorldHeight();

			assert(size.x > 0 || size.y > 0); // No size information was available - this is needed to create a fixture
		}

		bodyDef.position.Set(rend->getWorldX(), rend->getWorldY());
		bodyDef.angle = rend->getWorldRotation();

		bodyReference = world->CreateBody(&bodyDef);
		switch (bodyType)
		{
		case BodyType::Box:
		{
			b2PolygonShape shape;

			shape.SetAsBox(size.x / 2.f, size.y / 2.f);
			fixtureDef.shape = &shape;

			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Circle:
		{
			b2CircleShape shape;

			shape.m_radius = std::max(size.x, size.y) / 2.f;
			fixtureDef.shape = &shape;

			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Polygon:
		{
			const auto polygonPtr = std::dynamic_pointer_cast<AW::Polygon>(rend);
			if (polygonPtr != nullptr)
			{
				b2PolygonShape shape;

				const auto worldPoints = polygonPtr->getWorldPoints();
				std::vector<b2Vec2> pts;
				for (const auto& sp : worldPoints) pts.push_back(b2Vec2(sp.x, sp.y));

				shape.Set(&pts[0], (unsigned int)pts.size());
				fixtureDef.shape = &shape;

				bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		case BodyType::Line:
		{
			b2EdgeShape shape;

			const auto xOffset = size.x / 2.f;
			const auto yOffset = size.y / 2.f;

			b2Vec2 p1{ -xOffset, -yOffset }, p2{ xOffset, -yOffset };
			shape.Set(p1, p2);
			fixtureDef.shape = &shape;

			bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Chain:
		{
			const auto chainPtr = std::dynamic_pointer_cast<AW::Polygon>(rend);
			if (chainPtr!= nullptr)
			{
				b2ChainShape shape;

				const auto worldPoints = chainPtr->getWorldPoints();
				std::vector<b2Vec2> pts;
				for (const auto& sp : worldPoints) pts.push_back(b2Vec2(sp.x, sp.y));

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
			const auto rend = getRenderTargetFromListener();
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
		const auto rend = getRenderTargetFromListener();
		if (rend == nullptr)
		{
			return;
		}

		const auto pos = b2Vec2(rend->getWorldX(), rend->getWorldY());
		const auto rotation = rend->getWorldRotation();

		bodyReference->SetTransform(pos, rotation);
	}

	std::shared_ptr<SerializationClient> Body::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__body__", hint);

		bodyType = (BodyType)client->serializeInt("body-type", (int)bodyType);
		size.x = (float)client->serializeDouble("width", size.x);
		size.y = (float)client->serializeDouble("height", size.y);

		RigidBody::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}
}