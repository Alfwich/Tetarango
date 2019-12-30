#include "RigidBody.h"

#include <iostream>

namespace AW
{
	float RigidBody::screenToWorldPosition(float screen)
	{
		return screen / scalingFactor;
	}

	float RigidBody::worldToScreenPosition(float world)
	{
		return world * scalingFactor;
	}

	float RigidBody::screenToWorldRotation(float degrees)
	{
		return degrees * -(float)(AW::NumberHelper::PI / 180.0);
	}

	float RigidBody::worldToScreenRotation(float radians)
	{
		return radians * -(float)(180.0 / AW::NumberHelper::PI);
	}

	b2Body* RigidBody::createBody(const std::shared_ptr<b2World>& world)
	{
		bodyReference = onCreateBody(world);

		if (hasBody())
		{
			bodyReference->GetMassData(&massData);
			originalMass = massData.mass;
			massData.mass = originalMass * massFactor;
			bodyReference->SetMassData(&massData);

			physicUpdate();
		}

		return bodyReference;
	}

	bool RigidBody::hasBody()
	{
		return bodyReference != nullptr;
	}

	void RigidBody::physicUpdate()
	{
		if (hasBody())
		{
			doingPhysicUpdate = true;
			onPhysicUpdate();
			doingPhysicUpdate = false;
		}
	}

	void RigidBody::physicDetach(const std::shared_ptr<b2World>& world)
	{
		if (hasBody())
		{
			onPhysicDetach(world);
			world->DestroyBody(bodyReference);
			bodyReference = nullptr;
		}
	}

	void RigidBody::setDynamicBody()
	{
		bodyDef.type = b2_dynamicBody;
	}

	void RigidBody::setStaticBody()
	{
		bodyDef.type = b2_staticBody;
	}

	void RigidBody::applyForce(double vX, double vY, double amount)
	{
		if (hasBody())
		{
			bodyReference->ApplyForceToCenter(b2Vec2((float)(vX * amount), (float)(vY * amount)), true);
		}
	}

	void RigidBody::setWorldId(unsigned int worldId)
	{
		if (!hasBody())
		{
			this->worldId = worldId;
		}
	}

	unsigned int RigidBody::getWorldId()
	{
		return worldId;
	}

	void RigidBody::setMass(double m)
	{
		massFactor = (float)m;
		if (hasBody())
		{
			massData.mass = originalMass * massFactor;
			bodyReference->SetMassData(&massData);
		}
	}

	double RigidBody::getMass()
	{
		return massData.mass;
	}

	void RigidBody::setDensity(double d)
	{
		fixtureDef.density = (float)d;
		if (hasBody())
		{
			b2Fixture* fix = bodyReference->GetFixtureList();
			fix->SetDensity(fixtureDef.density);
		}
	}

	double RigidBody::getDensity()
	{
		return fixtureDef.density;
	}

	void RigidBody::setFriction(double f)
	{
		fixtureDef.friction = (float)f;
		if (hasBody())
		{
			b2Fixture* fix = bodyReference->GetFixtureList();
			fix->SetFriction(fixtureDef.friction);
		}
	}

	double RigidBody::getFriction()
	{
		return fixtureDef.friction;
	}

	void RigidBody::setFixedRotation(bool flag)
	{
		if (hasBody())
		{
			bodyReference->SetFixedRotation(flag);
		}
	}
}