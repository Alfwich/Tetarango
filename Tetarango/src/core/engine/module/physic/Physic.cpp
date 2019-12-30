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
			Logger::instance()->logCritical("Physic::Attempted to register world which already exists for worldId: " + std::to_string(worldId));
			return;
		}

		const auto world = std::make_shared<b2World>(b2Vec2((float)gravityX, (float)gravityY));
		const auto worldTimer = time->createTimer();
		worldTimer->start();

		worlds[worldId] = std::make_shared<WorldBundle>(world, worldTimer);
		Logger::instance()->log("Physic::Created world worldId: " + std::to_string(worldId));
	}

	void Physic::setWorldFps(unsigned int worldId, unsigned int fps)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set world fps for world id: " + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->timestep = 1.0 / (fps / 1.0);
	}

	void Physic::setWorldVelocityAndPositionIterations(unsigned int worldId, unsigned int velocity, unsigned int position)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to set world iterations for world id: " + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds.at(worldId)->velocityIterations = velocity;
		worlds.at(worldId)->positionIterations = position;
	}

	void Physic::registerRigidBodyForWorld(unsigned int worldId, std::shared_ptr<RigidBody> obj)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to register object for world id: " + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (obj == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to register null object for world id: " + std::to_string(worldId));
			return;
		}

		if (obj->hasBody())
		{
			Logger::instance()->logCritical("Physic::Attempted to register object for world id: " + std::to_string(worldId) + ", that already has a body");
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
			Logger::instance()->logCritical("Physic::Attempted to unregister body for world id: " + std::to_string(worldId) + ", that does not exist");
			return;
		}

		if (body == nullptr)
		{
			Logger::instance()->logCritical("Physic::Attempted to unregister null body for world id: " + std::to_string(worldId));
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

	void Physic::onInit()
	{
		registerWorld(0, 0.0, -20.0);
	}

	void Physic::onEnterFrame(const double& deltaTime)
	{
		for (const auto& worldIdToWorldBundle : worlds)
		{
			const auto& worldBundle = worldIdToWorldBundle.second;
			const auto threshold = (unsigned int)(worldBundle->timestep * 1000.0);
			if (threshold > 0 && worldBundle->worldTimer->isAboveThresholdAndRestart(threshold))
			{
				const auto& world = worldBundle->world;
				world->Step((float)worldBundle->timestep, worldBundle->velocityIterations, worldBundle->positionIterations);

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