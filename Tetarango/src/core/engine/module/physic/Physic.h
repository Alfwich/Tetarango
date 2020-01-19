#pragma once

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "engine/module/IBaseModule.h"
#include "engine/module/time/Time.h"
#include "engine/module/thread/Thread.h"

#include "RigidBody.h"
#include "RigidBodySensor.h"
#include "PhysicRenderer.h"

#include "ui/physic/body/Body.h"

namespace AW
{
	class Physic : public IBaseModule
	{
		SDL_sem* physicStepLock;

		bool  shouldStepOnBackgroundThread = false, doStep = false;

		PhysicRenderer physicRenderer;
		class RigidBodyBundle
		{
		public:
			RigidBodyBundle(std::weak_ptr<RigidBody> object, b2Body* body) : object(object), body(body) {}
			std::weak_ptr<RigidBody> object;
			b2Body* body;
		};

		class RigidBodyContactBundle
		{
		public:
			RigidBodyContactBundle(b2Fixture* a, b2Fixture* b, const std::shared_ptr<RigidBodySensor>& sensor) :a(a), b(b), bodyA(a->GetBody()), bodyB(b->GetBody()), sensor(sensor) {};
			b2Fixture* a, * b;
			b2Body* bodyA, * bodyB;
			const std::weak_ptr<RigidBodySensor> sensor;
		};

		class WorldBundle : public b2ContactListener, public b2DestructionListener
		{
		public:
			WorldBundle(std::shared_ptr<b2World> world, std::shared_ptr<Timer> timer) : world(world), worldTimer(timer) {};
			const std::shared_ptr<b2World> world;
			const std::shared_ptr<Timer> worldTimer;
			float timestep = 1 / 60.f;
			TimeScope timescope = TimeScope::Global;
			int velocityIterations = 6, positionIterations = 2;

			std::list<std::unique_ptr<RigidBodyBundle>> bodies;
			std::list<std::weak_ptr<RigidBodySensor>> sensors;

			std::vector<std::unique_ptr<RigidBodyContactBundle>> sensorsToNotifyBeginContact;
			std::vector<std::unique_ptr<RigidBodyContactBundle>> sensorsToNotifyEndContact;

			std::vector<b2Body*> bodiesToDestroy;

			virtual void BeginContact(b2Contact* contact) override;
			virtual void EndContact(b2Contact* contact) override;

			// Inherited via b2DestructionListener
			virtual void SayGoodbye(b2Joint* joint) override;
			virtual void SayGoodbye(b2Fixture* fixture) override;
		};

		std::shared_ptr<Time> time;
		std::shared_ptr<Thread> thread;

		std::unordered_map<unsigned int, std::unique_ptr<WorldBundle>> worlds;

		void startBackgroundThreadStepping();

	public:
		Physic();
		~Physic();

		void bindTime(std::shared_ptr<Time> time);
		void bindThread(std::shared_ptr<Thread> thread);

		void registerWorld(unsigned int worldId, double gravityX = 0.0, double gravityY = -9.807);

		void setWorldFps(unsigned int worldId, unsigned int timestepMS);
		void setWorldVelocityAndPositionIterations(unsigned int worldId, unsigned int velocity = 6, unsigned int position = 2);
		void setWorldGravity(unsigned int worldId, float gravityX = 0.f, float gravityY = -9.807f);
		void setWorldAllowSleeping(unsigned int worldId, bool flag);
		void setWorldTimescope(unsigned int worldId, TimeScope timescope = TimeScope::Global);

		double getPhysicFrameTime();

		void performDebugDraw(double screenWidth, double screenHeight);
		PhysicRenderer& getDebugRenderer();

		void registerRigidBodyForWorld(unsigned int worldId, const std::shared_ptr<RigidBody>& body);
		void unregisterRigidBodyForWorld(unsigned int worldId, b2Body* body);

		void registerRigidBodyJointForWorld(unsigned int worldId, const std::shared_ptr<RigidBodyJoint>& joint);

		void registerRigidBodySensorForWorld(unsigned int worldId, const std::shared_ptr<RigidBodySensor>& sensor);
		void unregisterRigidBodySensorForWorld(unsigned int worldId, const std::shared_ptr<RigidBodySensor>& sensor);

		void onInit();
		void onCleanup();
		void onEnterFrame(const double& deltaTime);
		void stepPhysicWorlds();

		bool backgroundThreadSteppingEnabled();
		void lockSimulations();
		void unlockSimulations();

		void markShouldStep();
		void markSteppedUnsafe();
		bool shouldStep();

	};
}
