#include "Physic.h"

namespace AW
{

	void Physic::bindTime(std::shared_ptr<Time> time)
	{
		this->time = time;
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

	void Physic::onInit()
	{
		registerWorld(0);
	}

	void Physic::onEnterFrame(const double& deltaTime)
	{
		for (const auto& worldIdToWorldBundle : worlds)
		{
			const auto& worldBundle = worldIdToWorldBundle.second;

			const auto timefactor = time->getComputedTimeFactor(worldBundle->timescope);
			if (timefactor <= 0.0) continue;

			const auto threshold = (unsigned int)(worldBundle->timestep * 1000.0);
			if (threshold > 0 && worldBundle->worldTimer->isAboveThresholdAndRestart(threshold))
			{
				const auto& world = worldBundle->world;
				world->Step(worldBundle->timestep * timefactor, worldBundle->velocityIterations, worldBundle->positionIterations);

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
			}
		}
	}
}