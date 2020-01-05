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

	void RigidBodySensor::beginContact(b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		if (!hasSensor() || bodyReference == nullptr) return;

		const auto bodyA = fixtureA->GetBody();
		const auto bodyB = fixtureB->GetBody();

		if ((bodyA == bodyReference || bodyB == bodyReference) && (fixtureA->IsSensor() || fixtureB->IsSensor()))
		{
			onBeginContact(bodyA, bodyB, fixtureA, fixtureB);
		}
	}

	void RigidBodySensor::endContact(b2Fixture * fixtureA, b2Fixture * fixtureB)
	{
		if (!hasSensor() || bodyReference == nullptr) return;

		const auto bodyA = fixtureA->GetBody();
		const auto bodyB = fixtureB->GetBody();

		if ((bodyA == bodyReference || bodyB == bodyReference) && (fixtureA->IsSensor() || fixtureB->IsSensor()))
		{
			onEndContact(bodyA, bodyB, fixtureA, fixtureB);
		}
	}
}
