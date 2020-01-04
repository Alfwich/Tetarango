#pragma once

#include <unordered_map>
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/time/Time.h"

#include "RigidBody.h"
#include "RigidBodySensor.h"

namespace AW
{
	class Physic : public IBaseModule, public b2ContactListener
	{
		class RigidBodyBundle
		{
		public:
			RigidBodyBundle(std::weak_ptr<RigidBody> object, b2Body *body) : object(object), body(body) {}
			std::weak_ptr<RigidBody> object;
			b2Body *body;
		};

		class WorldBundle
		{
		public:
			WorldBundle(std::shared_ptr<b2World> world, std::shared_ptr<Timer> timer) : world(world), worldTimer(timer) {};
			const std::shared_ptr<b2World> world;
			const std::shared_ptr<Timer> worldTimer;
			float timestep = 1 / 60.f;
			TimeScope timescope = TimeScope::Global;
			int velocityIterations = 6, positionIterations = 2;

			std::list<std::shared_ptr<RigidBodyBundle>> bodies;
			std::list<std::weak_ptr<RigidBodySensor>> sensors;
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
		void setWorldTimescope(unsigned int worldId, TimeScope timescope = TimeScope::Global);

		double getPhysicFrameDeltaTime();

		void registerRigidBodyForWorld(unsigned int worldId, const std::shared_ptr<RigidBody>& body);
		void unregisterRigidBodyForWorld(unsigned int worldId, b2Body* body);

		void registerRigidBodyJointForWorld(unsigned int worldId, const std::shared_ptr<RigidBodyJoint>& joint);

		void registerRigidBodySensorForWorld(unsigned int worldId, const std::shared_ptr<RigidBodySensor>& sensor);

		void onInit();
		void onEnterFrame(const double& deltaTime);

		/// Called when two fixtures begin to touch.
		void BeginContact(b2Contact* contact);

		/// Called when two fixtures cease to touch.
		void EndContact(b2Contact* contact);
	};
}
