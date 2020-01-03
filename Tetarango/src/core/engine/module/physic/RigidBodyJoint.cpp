#include "RigidBodyJoint.h"

namespace AW
{
	b2Joint* RigidBodyJoint::createJoint(const std::shared_ptr<b2World>& world)
	{
		return onCreateJoint(world);
	}

	b2Joint* RigidBodyJoint::getJoint()
	{
		if (!hasValidBodyReferences()) jointReference = nullptr;

		return jointReference;
	}

	bool RigidBodyJoint::hasJoint()
	{
		return getJoint() != nullptr;
	}

	void RigidBodyJoint::setWorldId(unsigned int worldId)
	{
		this->worldId = worldId;
	}

	unsigned int RigidBodyJoint::getWorldId()
	{
		return worldId;
	}

	std::shared_ptr<SerializationClient> RigidBodyJoint::doSerialize(SerializationHint hint)
	{
		return nullptr;
	}

	void RigidBodyJoint::doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient)
	{
		auto client = injectedClient->getClient("__rigid_joint__", hint);

		if (hint == SerializationHint::SERIALIZE && hasJoint())
		{
		}
		else
		{
		}
	}

}
