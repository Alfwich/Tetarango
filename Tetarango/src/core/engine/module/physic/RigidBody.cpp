#include "RigidBody.h"

namespace AW
{
	RigidBody::RigidBody()
	{
		cachedLV.SetZero();
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

			if (cachedLV.x != 0.f || cachedLV.y != 0.f)
			{
				bodyReference->SetLinearVelocity(cachedLV);
				cachedLV.SetZero();
			}

			if (cachedAV != 0.f)
			{
				bodyReference->SetAngularVelocity(cachedAV);
				cachedAV = 0.f;
			}

			if (cachedAD != 0.f)
			{
				bodyReference->SetAngularDamping(cachedAD);
				cachedAD = 0.f;
			}

			physicUpdate();
		}

		return bodyReference;
	}

	b2Body* RigidBody::getBody()
	{
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

	void RigidBody::applyForce(float vX, float vY, float amount)
	{
		if (hasBody())
		{
			bodyReference->ApplyForceToCenter(b2Vec2(vX * amount, vY * amount), true);
		}
	}

	void RigidBody::applyForce(AWVec2<float> v, float amount)
	{
		applyForce(v.x, v.y, amount);
	}

	void RigidBody::applyForce(float vX, float vY, float cX, float cY, float amount)
	{
		if (hasBody())
		{
			bodyReference->ApplyForce(b2Vec2(vX * amount, vY * amount), b2Vec2(cX, cY), true);
		}
	}

	void RigidBody::applyForce(AWVec2<float> v, AWVec2<float> c, float amount)
	{
		applyForce(v.x, v.y, c.x, c.y, amount);
	}

	void RigidBody::applyTorque(float t)
	{
		if (hasBody())
		{
			bodyReference->ApplyTorque(t, true);
		}
	}

	AWVec2<float> RigidBody::getVelocity()
	{
		auto result = AWVec2<float>();

		if (hasBody())
		{
			const auto v = bodyReference->GetLinearVelocity();
			result.x = v.x;
			result.y = v.y;
		}

		return result;
	}

	void RigidBody::doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient)
	{
		auto client = injectedClient->getClient("__rigid_body__", hint);

		bodyDef.type = (b2BodyType)client->serializeInt("body_type", (int)bodyDef.type);
		massFactor = (float)client->serializeDouble("massFactor", massFactor);
		fixtureDef.density = (float)client->serializeDouble("density", fixtureDef.density);
		bodyDef.fixedRotation = client->serializeBool("fixed-rot", bodyDef.fixedRotation);

		if (hint == SerializationHint::SERIALIZE && hasBody())
		{
			const auto lVelocity = bodyReference->GetLinearVelocity();
			client->setDouble("body-lv.x", lVelocity.x);
			client->setDouble("body-lv.y", lVelocity.y);
			client->setDouble("body-av", bodyReference->GetAngularVelocity());
			client->setDouble("body-ad", bodyReference->GetAngularDamping());
		}
		else
		{
			cachedLV.x = (float)client->getDouble("body-lv.x", 0.0);
			cachedLV.y = (float)client->getDouble("body-lv.y", 0.0);
			cachedAV = (float)client->getDouble("body-av", 0.0);
			cachedAD = (float)client->getDouble("body-ad", 0.0);
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
		bodyDef.fixedRotation = flag;
		if (hasBody())
		{
			bodyReference->SetFixedRotation(flag);
		}
	}
}