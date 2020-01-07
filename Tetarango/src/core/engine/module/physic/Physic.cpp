#include "Physic.h"

#include "box2d/b2_draw.h"

namespace AW
{


	Physic::Physic()
	{
		physicStepLock = SDL_CreateSemaphore(1);
	}

	Physic::~Physic()
	{
		SDL_DestroySemaphore(physicStepLock);
	}

	void Physic::bindTime(std::shared_ptr<Time> time)
	{
		this->time = time;
	}

	void Physic::bindThread(std::shared_ptr<Thread> thread)
	{
		this->thread = thread;
	}

	void Physic::registerWorld(unsigned int worldId, double gravityX, double gravityY)
	{
		if (worlds.count(worldId) == 1)
		{
			Logger::instance()->logCritical("Physic::Attempted to register world which already exists for worldId=" + std::to_string(worldId));
			return;
		}

		const auto world = std::make_shared<b2World>(b2Vec2((float)gravityX, (float)gravityY));
		const auto worldTimer = time->createTimer();
		worldTimer->start();

		worlds[worldId] = std::make_shared<WorldBundle>(world, worldTimer);
		worlds.at(worldId)->world->SetContactListener(worlds.at(worldId).get());
		worlds.at(worldId)->world->SetDebugDraw(&physicRenderer);
		Logger::instance()->log("Physic::Created world worldId=" + std::to_string(worldId));
	}

	void Physic::setWorldFps(unsigned int worldId, unsigned int fps)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set world fps for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->timestep = 1.f / (fps / 1.f);
	}

	void Physic::setWorldVelocityAndPositionIterations(unsigned int worldId, unsigned int velocity, unsigned int position)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set world iterations for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->velocityIterations = velocity;
		worlds.at(worldId)->positionIterations = position;
	}

	void Physic::setWorldGravity(unsigned int worldId, float gravityX, float gravityY)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set world gravity for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->world->SetGravity(b2Vec2(gravityX, gravityY));
	}

	void Physic::setWorldAllowSleeping(unsigned int worldId, bool flag)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set allow sleeping for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->world->SetAllowSleeping(flag);
	}

	void Physic::setWorldTimescope(unsigned int worldId, TimeScope timescope)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set timescope for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->timescope = timescope;
	}

	double Physic::getPhysicFrameDeltaTime()
	{
		return time->getDeltaTime();
	}

	void Physic::performDebugDraw(double screenWidth, double screenHeight)
	{
		physicRenderer.prepare(screenWidth, screenHeight);

		for (const auto& worldIdToWorldBundle : worlds)
		{
			const auto& worldBundle = worldIdToWorldBundle.second;
			worldBundle->world->DrawDebugData();
		}
	}

	PhysicRenderer & Physic::getDebugRenderer()
	{
		return physicRenderer;
	}

	void Physic::registerRigidBodyForWorld(unsigned int worldId, const std::shared_ptr<RigidBody>& obj)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to register object for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (obj == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to register null object for worldId=" + std::to_string(worldId));
			return;
		}

		if (obj->hasBody())
		{
			Logger::instance()->logCritical("Physic::Attempted to register object for worldId=" + std::to_string(worldId) + ", that already has a body");
			return;
		}

		auto worldBundle = worlds.at(worldId);
		const auto body = obj->createBody(worldBundle->world);
		worldBundle->bodies.push_back(std::make_shared<RigidBodyBundle>(obj, body));
	}

	void Physic::unregisterRigidBodyForWorld(unsigned int worldId, b2Body * body)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to unregister body for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (body == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to unregister null body for worldId=" + std::to_string(worldId));
			return;
		}

		const auto& worldBundle = worlds.at(worldId);
		const auto& world = worldBundle->world;

		for (auto rigidBodyBundle = worldBundle->bodies.begin(); rigidBodyBundle != worldBundle->bodies.end();)
		{
			if ((*rigidBodyBundle)->body == body)
			{
				const auto rigidBodyPtr = (*rigidBodyBundle)->object.lock();
				if (rigidBodyPtr != nullptr)
				{
					rigidBodyPtr->physicDetach(worldBundle->world);
				}
				else
				{
					world->DestroyBody(body);
				}
				worldBundle->bodies.erase(rigidBodyBundle);
				return;
			}
			else
			{
				++rigidBodyBundle;
			}
		}
	}

	void Physic::registerRigidBodyJointForWorld(unsigned int worldId, const std::shared_ptr<RigidBodyJoint>& obj)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to register joint for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (obj == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to register null joint for worldId=" + std::to_string(worldId));
			return;
		}

		if (obj->hasJoint())
		{
			Logger::instance()->logCritical("Physic::Attempted to register joint for worldId=" + std::to_string(worldId) + ", that already has a joint");
			return;
		}

		auto worldBundle = worlds.at(worldId);
		obj->createJoint(worldBundle->world);
	}

	void Physic::registerRigidBodySensorForWorld(unsigned int worldId, const std::shared_ptr<RigidBodySensor>& obj)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to register sensor for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (obj == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to register null sensor for worldId=" + std::to_string(worldId));
			return;
		}

		if (obj->hasSensor())
		{
			Logger::instance()->logCritical("Physic::Attempted to register sensor for worldId=" + std::to_string(worldId) + ", that already has a sensor");
			return;
		}

		auto worldBundle = worlds.at(worldId);
		worldBundle->sensors.push_back(obj);

		obj->createSensor();
	}

	void Physic::unregisterRigidBodySensorForWorld(unsigned int worldId, const std::shared_ptr<RigidBodySensor>& obj)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to unregister sensor for worldId=" + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (obj == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to unregister null sensor for worldId=" + std::to_string(worldId));
			return;
		}

		if (!obj->hasSensor())
		{
			Logger::instance()->logCritical("Physic::Attempted to unregister sensor for worldId=" + std::to_string(worldId) + ", that does not has a sensor");
			return;
		}

		const auto& worldBundle = worlds.at(worldId);

		for (auto it = worldBundle->sensors.begin(); it != worldBundle->sensors.end();)
		{
			const auto bodySensor = (*it).lock();
			if (bodySensor->getSensor() == obj->getSensor())
			{
				it = worldBundle->sensors.erase(it);
				return;
			}
			else
			{
				++it;
			}
		}
	}

	void Physic::stepPhysicWorlds()
	{
		AW_PROFILE_METHOD();
		for (const auto& worldIdToWorldBundle : worlds)
		{
			const auto& worldBundle = worldIdToWorldBundle.second;

			const auto timefactor = (float)time->getComputedTimeFactor(worldBundle->timescope);
			if (timefactor <= 0.0) continue;

			const auto threshold = (unsigned int)(worldBundle->timestep * 1000.0);
			if (threshold > 0 && worldBundle->worldTimer->isAboveThresholdAndRestart(threshold))
			{
				const auto& world = worldBundle->world;
				world->Step(worldBundle->timestep * timefactor, worldBundle->velocityIterations, worldBundle->positionIterations);
			}
		}
	}

	void Physic::startBackgroundThreadStepping()
	{
		thread->doWork(this, [](void* mod) {
			const auto physic = (Physic*)mod;

			while (physic->backgroundThreadSteppingEnabled())
			{
				if (physic->shouldStep())
				{
					physic->lockSimulations();
					physic->stepPhysicWorlds();
					physic->markSteppedUnsafe();
					physic->unlockSimulations();
				}
			}

			return (void*)1;
		}, weak_from_this());
	}

	void Physic::lockSimulations()
	{
		if (!shouldStepOnBackgroundThread) return;
		SDL_SemWait(physicStepLock);
	}

	void Physic::unlockSimulations()
	{
		if (!shouldStepOnBackgroundThread) return;
		SDL_SemPost(physicStepLock);
	}

	bool Physic::backgroundThreadSteppingEnabled()
	{
		return shouldStepOnBackgroundThread;
	}

	void Physic::markShouldStep()
	{
		lockSimulations();
		doStep = true;
		unlockSimulations();
	}

	void Physic::markSteppedUnsafe()
	{
		doStep = false;
	}

	bool Physic::shouldStep()
	{
		lockSimulations();
		const auto result = doStep;
		unlockSimulations();

		return shouldStepOnBackgroundThread && result;
	}

	void Physic::onInit()
	{
		registerWorld(0);

		// Only step on BG thread if we have extra threads for other operations
		shouldStepOnBackgroundThread = thread->hasExtraWorkerThreads();
		if (shouldStepOnBackgroundThread)
		{
			Logger::instance()->log("Physic::Running physic on background thread");
			startBackgroundThreadStepping();
		}
		else
		{
			Logger::instance()->log("Physic::Running physic on UI thread");
		}
	}

	void Physic::onCleanup()
	{
		shouldStepOnBackgroundThread = false;
	}

	void Physic::onEnterFrame(const double& deltaTime)
	{
		if (!shouldStepOnBackgroundThread) stepPhysicWorlds();

		for (const auto& worldIdToWorldBundle : worlds)
		{
			const auto& worldBundle = worldIdToWorldBundle.second;
			const auto& world = worldBundle->world;

			for (auto rigidBodyBundle = worldBundle->bodies.begin(); rigidBodyBundle != worldBundle->bodies.end();)
			{
				const auto rigidBodyPtr = (*rigidBodyBundle)->object.lock();
				if (rigidBodyPtr != nullptr)
				{
					rigidBodyPtr->physicUpdate();
					++rigidBodyBundle;
				}
				else
				{
					auto body = (*rigidBodyBundle)->body;
					world->DestroyBody(body);
					rigidBodyBundle = worldBundle->bodies.erase(rigidBodyBundle);
				}
			}

			for (const auto contactToSensorPair : worldBundle->sensorsToNotifyBeginContact)
			{
				const auto sensorPtr = contactToSensorPair->sensor.lock();
				if (sensorPtr != nullptr)
				{
					sensorPtr->beginContact(contactToSensorPair->a, contactToSensorPair->b);
				}
			}
			worldBundle->sensorsToNotifyBeginContact.clear();

			for (const auto contactToSensorPair : worldBundle->sensorsToNotifyEndContact)
			{
				const auto sensorPtr = contactToSensorPair->sensor.lock();
				if (sensorPtr != nullptr)
				{
					sensorPtr->endContact(contactToSensorPair->a, contactToSensorPair->b);
				}

			}
			worldBundle->sensorsToNotifyEndContact.clear();
		}

	}


}