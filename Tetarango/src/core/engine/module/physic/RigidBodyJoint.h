#pragma once

#include <memory>

#include "box2d/box2d.h"
#include "RigidBody.h"

namespace AW
{
	class RigidBodyJoint : public ISerializableDataSubscriber
	{

		unsigned int worldId = 0;

	protected:
		b2Joint* jointReference = nullptr;

	public:
		b2Joint* createJoint(const std::shared_ptr<b2World>& world);
		virtual b2Joint* onCreateJoint(const std::shared_ptr<b2World>& world) { return nullptr; };
		b2Joint* getJoint();
		bool hasJoint();
		virtual bool hasValidBodyReferences() = 0;

		void setWorldId(unsigned int worldId);
		unsigned int getWorldId();

		// Inherited via ISerializableDataSubscriber
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint) override;
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient);
	};
}
