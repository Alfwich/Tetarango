#include "RigidBodySensor.h"

namespace AW
{
	void RigidBodySensor::createSensor()
	{
		sensorFixtureReference = onCreateSensor();
		bodyReference = onGetBodyReference();
	}

	bool RigidBodySensor::hasSensor()
	{
		return sensorFixtureReference != nullptr;
	}

	void RigidBodySensor::onDetach()
	{
		sensorFixtureReference = nullptr;
		bodyReference = nullptr;
	}

	b2Fixture * RigidBodySensor::getSensor()
	{
		return sensorFixtureReference;
	}

	void RigidBodySensor::setWorldId(unsigned int worldId)
	{
		this->worldId = worldId;
	}

	unsigned int RigidBodySensor::getWorldId()
	{
		return worldId;
	}

	void RigidBodySensor::BeginContact(b2Contact *contact)
	{
		if (!hasSensor() || bodyReference == nullptr) return;

		const auto fixA = contact->GetFixtureA();
		const auto bodyA = fixA->GetBody();
		const auto fixB = contact->GetFixtureB();
		const auto bodyB = fixB->GetBody();

		if ((bodyA == bodyReference || bodyB == bodyReference) && (fixA->IsSensor() || fixB->IsSensor()))
		{
			onBeginContact(bodyA->GetUserData(), bodyB->GetUserData(), contact);
		}
	}

	void RigidBodySensor::EndContact(b2Contact *contact)
	{
		if (!hasSensor() || bodyReference == nullptr) return;

		const auto fixA = contact->GetFixtureA();
		const auto bodyA = fixA->GetBody();
		const auto fixB = contact->GetFixtureB();
		const auto bodyB = fixB->GetBody();

		if ((bodyA == bodyReference || bodyB == bodyReference) && (fixA->IsSensor() || fixB->IsSensor()))
		{
			onEndContact(bodyA->GetUserData(), bodyB->GetUserData(), contact);
		}
	}
}
