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
			Logger::instance()->logCritical("Physic::Attempted to set world timestamp for world id: " + std::to_string(worldId) + ", that does not exist");
			return;
		}

		worlds[worldId]->timestep = 1.0 / (fps / 1.0);
	}

	void Physic::registerRigidBodyForWorld(unsigned int worldId, std::shared_ptr<RigidBody> obj)
	{
		if (worlds.count(worldId) == 0)
		{
			Logger::instance()->logCritical("Physic::Attempted to register object for world id: " + std::to_string(worldId) + ", that does not exist");
			return;
		}

		auto worldBundle = worlds.at(worldId);
		auto& world = worldBundle->world;

		auto bodyDef = obj->onDefineBody();
		auto fixtureDef = obj->onDefineFixture();

		auto body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);

		obj->onPhysicUpdate(body);

		worldBundle->bodies.push_back(std::make_shared<RigidBodyBundle>(obj, body));
	}

	void Physic::onInit()
	{
		registerWorld(0, 0.0, -20.0);
	}

	void Physic::onEnterFrame(const double& deltaTime)
	{
		for (const auto worldIdToWorldBundle : worlds)
		{
			const auto& worldBundle = worldIdToWorldBundle.second;
			const auto threshold = (unsigned int)(worldBundle->timestep * 1000.0);
			if (threshold > 0 && worldBundle->worldTimer->isAboveThresholdAndRestart(threshold))
			{
				const auto& world = worldBundle->world;
				world->Step((float)worldBundle->timestep, worldBundle->velocityIterations, worldBundle->positionIterations);

				for (auto rigidBodyBundle = worldBundle->bodies.begin(); rigidBodyBundle != worldBundle->bodies.end();)
				{
					const auto body = (*rigidBodyBundle)->body;
					const auto rigidBodyPtr = (*rigidBodyBundle)->object.lock();
					if (rigidBodyPtr != nullptr)
					{
						rigidBodyPtr->onPhysicUpdate(body);
						rigidBodyBundle++;
					}
					else
					{
						world->DestroyBody(body);
						rigidBodyBundle = worldBundle->bodies.erase(rigidBodyBundle);
					}
				}
			}
		}
	}
}