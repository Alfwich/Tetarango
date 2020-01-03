#include "Joint.h"

namespace AW
{
	Joint::Joint()
	{
		registerGameObject<Joint>(__FUNCTION__);
	}

	void Joint::updateBindingReferences(const std::shared_ptr<IBodyListener>& bodyAObj, const std::shared_ptr<IBodyListener>& bodyBObj)
	{
		const auto goBodyAPtr = std::dynamic_pointer_cast<GameObject>(bodyAObj);
		if (goBodyAPtr != nullptr)
		{
			bodyABindingId = goBodyAPtr->getBindingId();
		}

		const auto goBodyBPtr = std::dynamic_pointer_cast<GameObject>(bodyBObj);
		if (goBodyBPtr != nullptr)
		{
			bodyBBindingId = goBodyBPtr->getBindingId();
		}
	}

	void Joint::ensureBodysAreAttached(const std::shared_ptr<IBodyListener>& bodyAObj, const std::shared_ptr<IBodyListener>& bodyBObj)
	{
		const auto bodyA = bodyAObj->getBodyObject();
		if (bodyA != nullptr && !bodyA->hasBody())
		{
			bodyA->onAttach();
		}

		const auto bodyB = bodyBObj->getBodyObject();
		if (bodyB != nullptr && !bodyB->hasBody())
		{
			bodyB->onAttach();
		}
	}

	std::shared_ptr<IBodyListener> Joint::getBodyAObj()
	{
		const auto bodyAWeakRefPtr = bodyA.lock();
		if (bodyAWeakRefPtr != nullptr) return bodyAWeakRefPtr;

		const auto parentPtr = parent.lock();
		if (parentPtr == nullptr) return nullptr;
		const auto bodyRef = parentPtr->findChildWithBindingId<IBodyListener>(bodyABindingId);

		bodyA = bodyRef;
		return bodyRef;
	}

	std::shared_ptr<IBodyListener> Joint::getBodyBObj()
	{
		const auto bodyBWeakRefPtr = bodyB.lock();
		if (bodyBWeakRefPtr != nullptr) return bodyBWeakRefPtr;

		const auto parentPtr = parent.lock();
		if (parentPtr == nullptr) return nullptr;
		const auto bodyRef = parentPtr->findChildWithBindingId<IBodyListener>(bodyBBindingId);

		bodyB = bodyRef;
		return bodyRef;
	}

	void Joint::onAttach()
	{
		if (!hasJoint())
		{
			const auto bodyAObj = getBodyAObj(), bodyBObj = getBodyBObj();

			if (bodyAObj != nullptr && bodyBObj != nullptr)
			{
				ensureBodysAreAttached(bodyAObj, bodyBObj);
				updateBindingReferences(bodyAObj, bodyBObj);
				modules->physic->registerRigidBodyJointForWorld(getWorldId(), std::dynamic_pointer_cast<RigidBodyJoint>(shared_from_this()));
			}
		}
	}

	void Joint::onDetach()
	{
		if (hasJoint())
		{
			jointReference = nullptr;
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