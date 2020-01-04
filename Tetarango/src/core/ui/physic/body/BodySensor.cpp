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
				collider.SetAsBox(RigidBody::screenToWorldPosition(width) / 2.f, RigidBody::screenToWorldPosition(height) / 2.f);
				for (auto v : collider.m_vertices)
				{
					v.x += RigidBody::screenToWorldPosition(x);
					v.y -= RigidBody::screenToWorldPosition(y);
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

	void BodySensor::onBeginContact(b2Contact * contact)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->BeginContact(contact);
		}
	}

	void BodySensor::onEndContact(b2Contact * contact)
	{
		const auto listenerPtr = listener.lock();
		if (listenerPtr != nullptr)
		{
			listenerPtr->EndContact(contact);
		}
	}
}
