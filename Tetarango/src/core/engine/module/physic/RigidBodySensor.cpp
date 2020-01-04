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

	void RigidBodySensor::BeginContact(b2Contact * contact)
	{
		const auto fixA = contact->GetFixtureA();
		const auto bodyA = fixA->GetBody();
		if (bodyA == bodyReference) return onBeginContact(contact);

		const auto fixB = contact->GetFixtureA();
		const auto bodyB = fixB->GetBody();
		if (bodyB == bodyReference) return onBeginContact(contact);
	}

	void RigidBodySensor::EndContact(b2Contact * contact)
	{
		const auto fixA = contact->GetFixtureA();
		const auto bodyA = fixA->GetBody();
		if (bodyA == bodyReference) return onEndContact(contact);

		const auto fixB = contact->GetFixtureA();
		const auto bodyB = fixB->GetBody();
		if (bodyB == bodyReference) return onEndContact(contact);
	}
}
