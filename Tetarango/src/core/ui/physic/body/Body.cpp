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

	std::shared_ptr<IBodyFixture> Body::getBodyFixtureFromListener()
	{
		return std::dynamic_pointer_cast<IBodyFixture>(listener.lock());
	}

	std::shared_ptr<Renderable> Body::getRenderTargetFromListener()
	{
		const auto lPtr = std::dynamic_pointer_cast<IBodyListener>(listener.lock());
		if (lPtr != nullptr) return lPtr->getRenderableBody();

		return nullptr;
	}

	void Body::createFixture(const std::shared_ptr<IBodyFixture>& fixture)
	{
		if (bodyReference == nullptr || fixture == nullptr) return;

		if (fixture->isSensor())
		{
			fixtureDef.isSensor = fixture->isSensor();
			fixtureDef.density = 0.0;
		}

		const auto worldSize = fixture->getBodyWorldSize();
		switch (fixture->getBodyType())
		{
		case BodyType::Box:
		{
			b2PolygonShape shape;

			shape.SetAsBox(worldSize.x / 2.f, worldSize.y / 2.f);
			fixtureDef.shape = &shape;

			fixture->fixtureReference = bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Circle:
		{
			b2CircleShape shape;

			shape.m_radius = std::max(worldSize.x, worldSize.y) / 2.f;
			fixtureDef.shape = &shape;

			fixture->fixtureReference = bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Line:
		{
			b2EdgeShape shape;

			const auto xOffset = worldSize.x / 2.f;
			const auto yOffset = worldSize.y / 2.f;

			b2Vec2 p1{ -xOffset, -yOffset }, p2{ xOffset, -yOffset };
			shape.Set(p1, p2);
			fixtureDef.shape = &shape;

			fixture->fixtureReference = bodyReference->CreateFixture(&fixtureDef);
		}
		break;

		case BodyType::Polygon:
		{
			const auto polygonPtr = std::dynamic_pointer_cast<AW::Polygon>(fixture);
			if (polygonPtr != nullptr)
			{
				b2PolygonShape shape;

				const auto worldPoints = polygonPtr->getWorldPoints();
				std::vector<b2Vec2> pts;
				for (const auto& sp : worldPoints) pts.push_back(b2Vec2(sp.x, sp.y));

				shape.Set(&pts[0], (unsigned int)pts.size());
				fixtureDef.shape = &shape;

				fixture->fixtureReference = bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		case BodyType::Chain:
		{
			const auto chainPtr = std::dynamic_pointer_cast<AW::Polygon>(fixture);
			if (chainPtr != nullptr)
			{
				b2ChainShape shape;

				const auto worldPoints = chainPtr->getWorldPoints();
				std::vector<b2Vec2> pts;
				for (const auto& sp : worldPoints) pts.push_back(b2Vec2(sp.x, sp.y));

				shape.CreateChain(&pts[0], (unsigned int)pts.size());
				fixtureDef.shape = &shape;

				fixture->fixtureReference = bodyReference->CreateFixture(&fixtureDef);
			}
		}
		break;

		}
	}

	void Body::setAutoUpdate(bool flag)
	{
		autoUpdate = flag;
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
			bodyReference = nullptr;

			for (const auto fixture : getChildrenOfType<IBodyFixture>())
			{
				fixture->fixtureReference = nullptr;
			}
		}
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

	void Body::add(std::shared_ptr<GameObject> obj)
	{
		GameObject::add(obj);

		const auto fixture = std::dynamic_pointer_cast<IBodyFixture>(obj);
		if (fixture != nullptr)
		{
			createFixture(fixture);
		}
	}

	void Body::remove(std::shared_ptr<GameObject> obj)
	{
		const auto fixture = std::dynamic_pointer_cast<IBodyFixture>(obj);
		if (fixture != nullptr && fixture->fixtureReference != nullptr && bodyReference != nullptr)
		{
			bodyReference->DestroyFixture(fixture->fixtureReference);
			fixture->fixtureReference = nullptr;
		}

		GameObject::remove(obj);
	}

	b2Body* Body::onCreateBody(const std::shared_ptr<b2World>& world)
	{
		const auto renderTarget = getRenderTargetFromListener();
		if (renderTarget == nullptr)
		{
			return nullptr;
		}

		bodyDef.position.Set(renderTarget->getWorldX(), renderTarget->getWorldY());
		bodyDef.angle = renderTarget->getWorldRotation();
		bodyReference = world->CreateBody(&bodyDef);
		bodyReference->SetUserData(this);

		const auto fixtureChildren = getChildrenOfType<IBodyFixture>();
		if (!fixtureChildren.empty())
		{
			for (const auto fixture : fixtureChildren)
			{
				createFixture(fixture);
			}
		}
		else
		{
			// Fallback to the listener as the fixture def
			const auto fixture = getBodyFixtureFromListener();
			if (fixture != nullptr)
			{
				createFixture(fixture);
			}
		}

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

		RigidBody::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}
}