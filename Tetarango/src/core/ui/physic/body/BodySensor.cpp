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
		if (!hasAttachedSensorCallback)
		{
			modules->physic->registerRigidBodySensorForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBodySensor>(shared_from_this()));
			hasAttachedSensorCallback = true;
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
		if (hasAttachedSensorCallback)
		{
			modules->physic->unregisterRigidBodySensorForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBodySensor>(shared_from_this()));
			hasAttachedSensorCallback = false;
		}
	}

	void BodySensor::onBeginContact(b2Body* bodyA, b2Body* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			const auto bodyRef = listenerPtr->getBodyObject()->getBody();
			if (bodyA == bodyRef || bodyB == bodyRef)
			{
				listenerPtr->onBeginContact(std::make_unique<ContactBundle>(bodyA->GetUserData(), bodyB->GetUserData(), fixtureA, fixtureB));
			}
		}
	}

	void BodySensor::onEndContact(b2Body* bodyA, b2Body* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			const auto bodyRef = listenerPtr->getBodyObject()->getBody();
			if (bodyA == bodyRef || bodyB == bodyRef)
			{
				listenerPtr->onEndContact(std::make_unique<ContactBundle>(bodyA->GetUserData(), bodyB->GetUserData(), fixtureA, fixtureB));
			}
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

	const BodyType BodySensor::getBodyType() const
	{
		return BodyType::Box;
	}

	const AWVec2<float> BodySensor::getBodyWorldSize()
	{
		return AWVec2<float>(rect.w, rect.h);
	}

	const bool BodySensor::isSensor() const
	{
		return true;
	}
}
