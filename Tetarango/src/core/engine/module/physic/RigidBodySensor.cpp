#include "RigidBodySensor.h"

namespace AW
{
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
		const auto bodyA = fixtureA->GetBody();
		const auto bodyB = fixtureB->GetBody();

		onBeginContact(bodyA, bodyB, fixtureA, fixtureB);
	}

	void RigidBodySensor::endContact(b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		const auto bodyA = fixtureA->GetBody();
		const auto bodyB = fixtureB->GetBody();

		onEndContact(bodyA, bodyB, fixtureA, fixtureB);
	}
}
