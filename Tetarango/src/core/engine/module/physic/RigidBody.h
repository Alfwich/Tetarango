#pragma once

#include "box2d/box2d.h"
#include "engine/module/serialization/ISerializable.h"
#include "RigidBodyJoint.h"

namespace AW
{
	class RigidBody : public ISerializableDataSubscriber
	{

		float cachedAV = 0.f, cachedAD = 0.f, density = 1.f, friction = 0.3f;
		unsigned int worldId = 0;

		b2Vec2 cachedLV;

	protected:

		bool doingPhysicUpdate = false;
		float mass = 0.f;
		b2BodyDef bodyDef;

		b2Body* bodyReference = nullptr;

	public:
		RigidBody();

		b2Body* createBody(const std::shared_ptr<b2World>& world);
		virtual b2Body* onCreateBody(const std::shared_ptr<b2World>& world) { return nullptr; };
		b2Body* getBody();
		bool hasBody();

		void physicUpdate();
		virtual void onPhysicUpdate() { /* NO-OP */ };

		void physicDetach(const std::shared_ptr<b2World>& world);
		virtual void onPhysicDetach(const std::shared_ptr<b2World>& world) { /* NO-OP */ };

		void setWorldId(unsigned int worldId);
		unsigned int getWorldId();

		void setMass(double m);
		double getMass();

		void setDensity(double d);
		double getDensity();

		void setFriction(double f);
		double getFriction();

		void setFixedRotation(bool flag);

		void setDynamicBody();
		void setStaticBody();

		virtual void applyForce(float vX, float vY, float amount);
		virtual void applyForce(AWVec2<float> v, float amount);

		virtual void applyForce(float vX, float vY, float cX, float cY, float amount);
		virtual void applyForce(AWVec2<float> v, AWVec2<float> c, float amount);

		virtual void applyTorque(float t);

		virtual AWVec2<float> getVelocity();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint) { return nullptr; };
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient);
	};
}
