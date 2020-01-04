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

	void BodySensor::setX(double x)
	{
		rect.x = x;
	}

	double BodySensor::getX()
	{
		return rect.x;
	}

	void BodySensor::setY(double y)
	{
		rect.y = y;
	}

	double BodySensor::getY()
	{
		return rect.y;
	}

	void BodySensor::setWidth(double w)
	{
		rect.w = w;
	}

	double BodySensor::getWidth()
	{
		return rect.w;
	}

	void BodySensor::setHeight(double h)
	{
		rect.h = h;
	}

	double BodySensor::getHeight()
	{
		return rect.h;
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
		RigidBodySensor::onDetach();
	}

	b2Fixture * BodySensor::onCreateSensor()
	{
		const auto parentBodyPtr = std::dynamic_pointer_cast<Body>(parent.lock());
		if (parentBodyPtr != nullptr)
		{
			const auto bodyPtr = parentBodyPtr->getBody();
			if (bodyPtr != nullptr)
			{
				b2FixtureDef fixtureDef;

				b2PolygonShape collider;
				collider.SetAsBox(RigidBody::screenToWorldPosition(rect.w) / 2.f, RigidBody::screenToWorldPosition(rect.h) / 2.f);
				for (auto v : collider.m_vertices)
				{
					v.x += RigidBody::screenToWorldPosition(rect.x);
					v.y -= RigidBody::screenToWorldPosition(rect.y);
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

	void BodySensor::onBeginContact(void* bodyA, void* bodyB, b2Contact * contact)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onBeginContact(static_cast<Body*>(bodyA), static_cast<Body*>(bodyB), contact);
		}
	}

	void BodySensor::onEndContact(void* bodyA, void* bodyB, b2Contact * contact)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->onEndContact(static_cast<Body*>(bodyA), static_cast<Body*>(bodyB), contact);
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
