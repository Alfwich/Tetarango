#include "Joint.h"

namespace AW
{

	Joint::Joint()
	{
		registerGameObject<Joint>(__FUNCTION__);
	}

	std::shared_ptr<IBodyListener> Joint::findBodyListenerWithBindingId(int id)
	{
		const auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			return std::dynamic_pointer_cast<IBodyListener>(parentPtr->findChildWithBindingId(id));
		}

		return nullptr;
	}

	void Joint::onAttach()
	{
		if (!hasJoint())
		{
			auto bodyAPtr = bodyA.lock(), bodyBPtr = bodyB.lock();
			if (bodyAPtr == nullptr && bodyABindingId != 0)
			{
				auto rebindedA = findBodyListenerWithBindingId(bodyABindingId);
				bodyA = rebindedA;
				bodyAPtr = rebindedA;
			}

			if (bodyBPtr == nullptr && bodyBBindingId != 0)
			{
				auto rebindedB = findBodyListenerWithBindingId(bodyBBindingId);
				bodyB = rebindedB;
				bodyBPtr = rebindedB;
			}

			if (bodyAPtr != nullptr && bodyBPtr != nullptr)
			{
				const auto bodyAObj = bodyAPtr->getBodyObject();
				if (!bodyAObj->hasBody())
				{
					bodyAObj->onAttach();
				}

				const auto bodyBObj = bodyBPtr->getBodyObject();
				if (!bodyBObj->hasBody())
				{
					bodyBObj->onAttach();
				}

				const auto bodyAObjParent = bodyAObj->getParent().lock();
				if (bodyAObjParent != nullptr)
				{
					bodyABindingId = bodyAObj->getParent().lock()->getBindingId();
				}

				const auto bodyBObjParent = bodyBObj->getParent().lock();
				if (bodyBObjParent != nullptr)
				{
					bodyBBindingId = bodyBObj->getParent().lock()->getBindingId();
				}

				modules->physic->registerRigidBodyJointForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBodyJoint>(shared_from_this()));
			}
		}
	}

	b2Joint * Joint::onCreateJoint(const std::shared_ptr<b2World>& world)
	{
		const auto bodyAPtr = bodyA.lock(), bodyBPtr = bodyB.lock();
		if (bodyAPtr != nullptr && bodyBPtr != nullptr)
		{
			const auto bodyAObj = bodyAPtr->getBodyObject(), bodyBObj = bodyBPtr->getBodyObject();

			const auto anchor = bodyAObj->getBody()->GetWorldCenter();
			b2RevoluteJointDef jointDef;
			jointDef.Initialize(bodyAObj->getBody(), bodyBObj->getBody(), anchor);
			jointDef.localAnchorB.x = 2.0;
			jointDef.localAnchorB.y = 2.0;
			jointReference = world->CreateJoint(&jointDef);
		}

		return jointReference;
	}

	std::shared_ptr<SerializationClient> Joint::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__joint__", hint);

		bodyABindingId = client->serializeInt("b-a-b-i", bodyABindingId);
		bodyBBindingId = client->serializeInt("b-b-b-i", bodyBBindingId);

		RigidBodyJoint::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}
}