#include "RigidBody.h"

#include <iostream>

namespace AW
{
	float RigidBody::screenToWorldPosition(float pixel)
	{
		return pixel / scalingFactor;
	};

	float RigidBody::worldToScreenPosition(float world)
	{
		return world * scalingFactor;
	};

	float RigidBody::screenToWorldRotation(float degrees)
	{
		return degrees * -(float)(AW::NumberHelper::PI / 180.0);
	};

	float RigidBody::worldToScreenRotation(float radians)
	{
		return radians * -(float)(180.0 / AW::NumberHelper::PI);
	}

	void RigidBody::onBindBody(b2Body * bodyReference)
	{
		this->bodyReference = bodyReference;
	}

	void RigidBody::physicUpdate()
	{
		if (bodyReference != nullptr)
		{
			onPhysicUpdate();
		}
	}

	void RigidBody::applyForce(float vX, float vY, float amount)
	{
		if (bodyReference != nullptr)
		{
			bodyReference->ApplyForceToCenter(b2Vec2(vX * amount, vY * amount), true);
		}
	}

	void RigidBody::setMass(float m)
	{
		if (bodyReference != nullptr)
		{
			b2MassData mass;
			bodyReference->GetMassData(&mass);
			mass.mass = m;
			bodyReference->SetMassData(&mass);
		}
	}

	void RigidBody::setFixedRotation(bool flag)
	{
		if (bodyReference != nullptr)
		{
			bodyReference->SetFixedRotation(flag);
		}
	}

}