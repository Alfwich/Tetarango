#pragma once

#include <unordered_map>
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/time/Time.h"
#include "RigidBody.h"
#include "RigidBodyJoint.h"

namespace AW
{
	class Physic : public IBaseModule
	{
		class RigidBodyBundle
		{
		public:
			RigidBodyBundle(std::weak_ptr<RigidBody> object, b2Body *body) : object(object), body(body) {}
			std::weak_ptr<RigidBody> object;
			b2Body *body;
		};

		class RigidBodyJointBundle
		{
		public:
			RigidBodyJointBundle(std::weak_ptr<RigidBodyJoint> object, b2Joint *joint) : object(object), joint(joint) {}
			std::weak_ptr<RigidBodyJoint> object;
			b2Joint *joint;
		};


		class WorldBundle
		{
		public:
			WorldBundle(std::shared_ptr<b2World> world, std::shared_ptr<Timer> timer) : world(world), worldTimer(timer) {};
			const std::shared_ptr<b2World> world;
			const std::shared_ptr<Timer> worldTimer;
			float timestep = 1 / 60.f;
			int velocityIterations = 6, positionIterations = 2;

			std::list<std::shared_ptr<RigidBodyBundle>> bodies;
		};

		std::shared_ptr<Time> time;

		std::unordered_map<unsigned int, std::shared_ptr<WorldBundle>> worlds;

	public:

		void bindTime(std::shared_ptr<Time> time);

		void registerWorld(unsigned int worldId, double gravityX = 0.0, double gravityY = -9.807);

		void setWorldFps(unsigned int worldId, unsigned int timestepMS);
		void setWorldVelocityAndPositionIterations(unsigned int worldId, unsigned int velocity = 6, unsigned int position = 2);
		void setWorldGravity(unsigned int worldId, float gravityX = 0.f, float gravityY = -9.807f);
		void setWorldAllowSleeping(unsigned int worldId, bool flag);

		void registerRigidBodyForWorld(unsigned int worldId, const std::shared_ptr<RigidBody>& body);
		void unregisterRigidBodyForWorld(unsigned int worldId, b2Body* body);

		void registerRigidBodyJointForWorld(unsigned int worldId, const std::shared_ptr<RigidBodyJoint>& joint);

		void onInit();
		void onEnterFrame(const double& deltaTime);
	};
}
