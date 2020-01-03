#include "Joint.h"

namespace AW
{
	Joint::Joint()
	{
		registerGameObject<Joint>(__FUNCTION__);
	}

	void Joint::setJointType(JointType type)
	{
		jointType = type;
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

	std::shared_ptr<IBodyListener> Joint::getBodyAListener()
	{
		const auto bodyAWeakRefPtr = bodyA.lock();
		if (bodyAWeakRefPtr != nullptr) return bodyAWeakRefPtr;

		const auto parentPtr = parent.lock();
		if (parentPtr == nullptr) return nullptr;
		const auto bodyRef = parentPtr->findChildWithBindingId<IBodyListener>(bodyABindingId);

		bodyA = bodyRef;
		return bodyRef;
	}

	std::shared_ptr<IBodyListener> Joint::getBodyBListener()
	{
		const auto bodyBWeakRefPtr = bodyB.lock();
		if (bodyBWeakRefPtr != nullptr) return bodyBWeakRefPtr;

		const auto parentPtr = parent.lock();
		if (parentPtr == nullptr) return nullptr;
		const auto bodyRef = parentPtr->findChildWithBindingId<IBodyListener>(bodyBBindingId);

		bodyB = bodyRef;

		return bodyRef;
	}

	std::shared_ptr<Body> Joint::getBodyA()
	{
		const auto bodyAObj = getBodyAListener();
		return bodyAObj != nullptr ? bodyAObj->getBodyObject() : nullptr;
	}

	std::shared_ptr<Body> Joint::getBodyB()
	{
		const auto bodyBObj = getBodyBListener();
		return bodyBObj != nullptr ? bodyBObj->getBodyObject() : nullptr;
	}

	void Joint::onAttach()
	{
		if (!hasJoint())
		{
			const auto bodyAObj = getBodyAListener(), bodyBObj = getBodyBListener();

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
		const auto bodyA = getBodyA(), bodyB = getBodyB();
		if (bodyA == nullptr || bodyB == nullptr)
		{
			return nullptr;
		}

		switch (jointType)
		{
		case JointType::Revolute:
		{
			const auto anchor = bodyA->getBody()->GetWorldCenter();
			b2RevoluteJointDef jointDef;
			jointDef.Initialize(bodyA->getBody(), bodyB->getBody(), anchor);
			jointDef.localAnchorB.x = (float)jointDistance;
			jointDef.localAnchorB.y = (float)jointDistance;
			jointReference = world->CreateJoint(&jointDef);
		}
		break;

		case JointType::Distant:
		{
			const auto anchorA = bodyA->getBody()->GetWorldCenter();
			const auto anchorB = bodyB->getBody()->GetWorldCenter();
			b2DistanceJointDef jointDef;
			jointDef.Initialize(bodyA->getBody(), bodyB->getBody(), anchorA, anchorB);
			jointReference = world->CreateJoint(&jointDef);
		}
		break;

		default:
			break;

		}

		return jointReference;
	}

	void Joint::setJointDistance(float distance)
	{
		jointDistance = RigidBody::screenToWorldPosition(distance);
	}

	double Joint::getJointDistance()
	{
		return RigidBody::worldToScreenPosition(jointDistance);
	}

	std::shared_ptr<SerializationClient> Joint::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__joint__", hint);

		jointType = (JointType)client->serializeInt("joint-t", (int)jointType);
		jointDistance = (float)client->serializeDouble("joint-d", jointDistance);
		bodyABindingId = client->serializeInt("b-a-b-i", bodyABindingId);
		bodyBBindingId = client->serializeInt("b-b-b-i", bodyBBindingId);

		RigidBodyJoint::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}

	bool Joint::hasValidBodyReferences()
	{
		return getBodyA() != nullptr && getBodyB() != nullptr;
	}
}