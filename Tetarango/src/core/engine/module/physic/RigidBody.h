#pragma once

#include "box2d/box2d.h"
#include "util/NumberHelper.h"
#include "util/Vec.h"
#include "engine/module/serialization/ISerializable.h"
#include "RigidBodyJoint.h"

namespace AW
{
	class RigidBody : public ISerializableDataSubscriber
	{

		float originalMass = 0.f, cachedAV = 0.f, cachedAD = 0.f;
		unsigned int worldId = 0;

		b2Vec2 cachedLV;

	protected:

		bool doingPhysicUpdate = false;
		float massFactor = 1.0;
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
		b2MassData massData;

		b2Body* bodyReference = nullptr;

	public:
		static const float scalingFactor;

		static float screenToWorldPosition(float screen);
		static float worldToScreenPosition(float world);

		static float screenToWorldRotation(float degrees);
		static float worldToScreenRotation(float radians);

		static AWVec2<float> worldToScreen(const AWVec2<float>& world);
		static AWVec2<float> screenToWorld(const AWVec2<float>& screen);

		static AWVec2<float> worldToScreen(const AWVec2<double>& world);
		static AWVec2<float> screenToWorld(const AWVec2<double>& screen);

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

		virtual void applyForce(double vX, double vY, double amount);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint) { return nullptr; };
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient);
	};
}
