#include "BodySensor.h"

#include "Body.h"

namespace
{
	const auto bodyListenerBindingIdName = "b-s-b-l-binding-id";
}

namespace AW
{
	BodySensor::BodySensor()
	{
		GORegister(BodySensor);
	}

	void BodySensor::setWorldX(float x)
	{
		rect.x = x;
	}

	float BodySensor::getWorldX()
	{
		return (float)rect.x;
	}

	void BodySensor::setWorldY(float y)
	{
		rect.y = y;
	}

	float BodySensor::getWorldY()
	{
		return (float)rect.y;
	}

	void BodySensor::setWorldWidth(float w)
	{
		rect.w = w;
	}

	float BodySensor::getWorldWidth() const
	{
		return (float)rect.w;
	}

	void BodySensor::setWorldHeight(float h)
	{
		rect.h = h;
	}

	void BodySensor::setScreenWidth(float w)
	{
		rect.w = Renderable::screenToWorldPosition(w);
	}

	void BodySensor::setScreenHeight(float h)
	{
		rect.h = Renderable::screenToWorldPosition(h);
	}

	float BodySensor::getWorldHeight() const
	{
		return (float)rect.h;
	}

	void BodySensor::onAttach()
	{
		if (!hasSensor())
		{
			modules->physic->registerRigidBodySensorForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBodySensor>(shared_from_this()));
		}

		const auto listenerPtr = listener.lock();
		if (listenerPtr == nullptr)
		{
			const auto bindingId = serializationClient->getInt(bodyListenerBindingIdName);
			if (bindingId != 0)
			{
				listener = getRootNode()->findChildWithBindingId<IBodyListener>(bindingId);
			}
			else
			{
				listener = findFirstInParentChain<IBodyListener>();
			}
		}

		const auto goPtr = std::dynamic_pointer_cast<AW::GameObject>(listener.lock());
		if (goPtr != nullptr)
		{
			serializationClient->setInt(bodyListenerBindingIdName, goPtr->getBindingId());
		}
	}

	void BodySensor::onDetach()
	{
		if (hasSensor())
		{
			modules->physic->unregisterRigidBodySensorForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBodySensor>(shared_from_this()));
		}

		RigidBodySensor::onDetach();
	}

	b2Fixture * BodySensor::onCreateSensor()
	{
		const auto parentBodyPtr = std::dynamic_pointer_cast<Body>(parent.lock());
		if (parentBodyPtr != nullptr)
		{
			auto bodyPtr = parentBodyPtr->getBody();
			if (bodyPtr == nullptr) parentBodyPtr->onAttach();

			bodyPtr = parentBodyPtr->getBody();
			if (bodyPtr != nullptr)
			{
				b2FixtureDef fixtureDef;

				b2PolygonShape collider;
				collider.SetAsBox((float)rect.w / 2.f, (float)rect.h / 2.f);
				for (auto v : collider.m_vertices)
				{
					v.x += (float)rect.x;
					v.y -= (float)rect.y;
				}

				fixtureDef.shape = &collider;
				fixtureDef.isSensor = true;

				return bodyPtr->CreateFixture(&fixtureDef);
			}
		}

		return nullptr;
	}

	b2Body* BodySensor::onGetBodyReference()
	{
		const auto parentBodyPtr = std::dynamic_pointer_cast<Body>(parent.lock());
		if (parentBodyPtr != nullptr)
		{
			const auto bodyPtr = parentBodyPtr->getBody();
			if (bodyPtr != nullptr)
			{
				return bodyPtr;
			}
		}

		return nullptr;
	}

	void BodySensor::onBeginContact(b2Body *bodyA, b2Body *bodyB, b2Fixture *fixtureA, b2Fixture *fixtureB)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onBeginContact(std::make_unique<ContactBundle>(bodyA->GetUserData(), bodyB->GetUserData(), fixtureA, fixtureB));
		}
	}

	void BodySensor::onEndContact(b2Body *bodyA, b2Body *bodyB, b2Fixture *fixtureA, b2Fixture *fixtureB)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onEndContact(std::make_unique<ContactBundle>(bodyA->GetUserData(), bodyB->GetUserData(), fixtureA, fixtureB));
		}
	}

	std::shared_ptr<SerializationClient> BodySensor::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__sensor__", hint);

		rect.x = client->serializeDouble("r.x", rect.x);
		rect.y = client->serializeDouble("r.y", rect.y);
		rect.w = client->serializeDouble("r.w", rect.w);
		rect.h = client->serializeDouble("r.h", rect.h);

		return GameObject::doSerialize(hint);
	}
}
