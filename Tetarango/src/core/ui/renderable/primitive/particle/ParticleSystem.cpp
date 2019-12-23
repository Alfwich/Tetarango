#include "ParticleSystem.h"


namespace AW
{
	ParticleSystem::ParticleSystem()
	{
		disableCulling = true;
		renderType = RenderType::ParticleSystem;
		registerSerialization<ParticleSystem>();
	}

	void ParticleSystem::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-color" });
	}

	void ParticleSystem::onInitialAttach()
	{
		frameTimer = modules->time->createTimer(getTimeScope(), true);
		enableEnterFrame();

		if (started)
		{
			frameTimer->start();
		}
	}

	void ParticleSystem::preRender(Rect* renderRect, RenderPackage* renderPackage)
	{
		const auto globalRef = globalReference.lock();
		for (const auto particle : particles)
		{
			if (isGloballyEmitting && globalRef != nullptr)
			{
				particle->x = particle->x + renderRect->x;
				particle->y = particle->y + renderRect->y;
				globalRef->acceptParticleFromLocalSystem(particle);
			}
		}

		if (isGloballyEmitting && globalRef != nullptr)
		{
			particles.clear();
		}
	}

	const std::list<std::shared_ptr<Particle>> ParticleSystem::getParticles()
	{
		return particles;
	}

	void ParticleSystem::setParticleFactory(std::shared_ptr<IParticleFactory> factory)
	{
		particleFactory = factory;
		add(factory);
	}

	void ParticleSystem::setParticlesPerSecond(int particlesPerSecond)
	{
		this->particlesPerSecond = particlesPerSecond;
	}

	void ParticleSystem::setSystemVelocity(double xVelocity, double yVelocity)
	{
		systemVelocityX = xVelocity;
		systemVelocityY = yVelocity;
	}

	void ParticleSystem::clear()
	{
		particles.clear();
	}

	void ParticleSystem::setGlobalEmission(bool value)
	{
		isGloballyReceiving = false;
		isGloballyEmitting = value;
		hasCheckedForGlobalSystem = false;
	}

	void ParticleSystem::setGlobalReceiver(bool value)
	{
		isGloballyEmitting = false;
		isGloballyReceiving = value;
	}

	void ParticleSystem::disableGlobalInteractions()
	{
		isGloballyEmitting = false;
		isGloballyReceiving = false;
	}

	void ParticleSystem::onEnterFrame(const double& frameTime)
	{
		double deltaTime = frameTime / 1000.0;
		for (auto it = particles.begin(); it != particles.end();)
		{
			const auto particle = (*it);
			if (particle->shouldDecay)
			{
				particle->timeLeft -= frameTime;
			}

			if (particle->timeLeft < 0.0)
			{
				it = particles.erase(it);
			}
			else
			{
				particle->onEnterFrame(deltaTime);
				++it;
			}
		}

		if (!started)
		{
			return;
		}

		if (particlesPerSecond > 0 && particleFactory != nullptr && !isGloballyReceiving)
		{
			auto ticks = frameTimer->getTicks();
			auto ppS = 1000.0 / particlesPerSecond;
			if (ticks >= ppS)
			{
				ParticleCreationBundle bundle
				{
					getRect(),
					systemVelocityX,
					systemVelocityY,
					getRotation(),
					getAlpha()
				};

				while (ticks >= 1000.0 / particlesPerSecond)
				{
					emitParticle(bundle, particleFactory);
					ticks -= ppS;
				}

				frameTimer->start();
			}
		}
	}

	void ParticleSystem::emitParticle(ParticleCreationBundle& bundle, const std::shared_ptr<IParticleFactory>& factory)
	{
		const auto particle = factory->makeParticle(bundle);


		if (isGloballyEmitting && !hasCheckedForGlobalSystem)
		{
			hasCheckedForGlobalSystem = true;
			if (!findGlobalEmitter())
			{
				isGloballyEmitting = false;
			}
		}

		particles.push_back(particle);
	}

	bool ParticleSystem::findGlobalEmitter()
	{
		const auto emitter = findClosestGlobalSystem();

		if (emitter)
		{
			globalReference = emitter;
			return true;
		}

		return false;
	}

	std::shared_ptr<ParticleSystem> ParticleSystem::findClosestGlobalSystem()
	{
		auto currentTarget = parent.lock();

		while (currentTarget != nullptr)
		{
			if (checkCandidateAO(currentTarget))
			{
				return std::dynamic_pointer_cast<ParticleSystem>(currentTarget);
			}

			for (const auto child : currentTarget->getChildren())
			{
				if (checkCandidateAO(child))
				{
					return std::dynamic_pointer_cast<ParticleSystem>(child);
				}
			}

			if (currentTarget->isRootElement())
			{
				return nullptr;
			}

			currentTarget = currentTarget->getParent().lock();
		}

		return nullptr;
	}

	bool ParticleSystem::checkCandidateAO(std::shared_ptr<GameObject> candiate)
	{
		if (candiate->getId() == getId())
		{
			return false;
		}

		const auto casted = std::dynamic_pointer_cast<ParticleSystem>(candiate);
		if (casted == nullptr)
		{
			return false;
		}

		if (casted->isGloballyReceiving)
		{
			return true;
		}

		return false;
	}

	void ParticleSystem::acceptParticleFromLocalSystem(std::shared_ptr<Particle> particle)
	{
		particles.push_back(particle);
	}

	void ParticleSystem::onAttach()
	{
		frameTimer->setTimeScope(getTimeScope());
	}

	void ParticleSystem::onDetach()
	{
		stop();
	}

	void ParticleSystem::onTimeScopeChanged()
	{
		if (frameTimer == nullptr)
		{
			return;
		}

		frameTimer->setTimeScope(getTimeScope());
	}

	void ParticleSystem::start()
	{
		if (started)
		{
			return;
		}

		started = true;
		if (frameTimer == nullptr)
		{
			return;
		}

		frameTimer->start();
	}

	void ParticleSystem::emitImmediately(int numberOfParticles)
	{
		if (particleFactory == nullptr || numberOfParticles == 0)
		{
			return;
		}

		ParticleCreationBundle bundle
		{
			getRect(),
			systemVelocityX,
			systemVelocityY,
			getRotation(),
			getAlpha()
		};

		for (int i = 0; i < numberOfParticles; ++i)
		{
			emitParticle(bundle, particleFactory);
		}

		preRender(&worldRect, nullptr);
	}

	void ParticleSystem::emitImmediatelyWithFactory(int numberOfParticles, const std::shared_ptr<IParticleFactory>& factory)
	{
		if (factory == nullptr || numberOfParticles == 0)
		{
			return;
		}

		ParticleCreationBundle bundle
		{
			getRect(),
			systemVelocityX,
			systemVelocityY,
			getRotation(),
			getAlpha()
		};

		for (int i = 0; i < numberOfParticles; ++i)
		{
			emitParticle(bundle, factory);
		}

		preRender(&worldRect, nullptr);
	}

	void ParticleSystem::stop()
	{
		if (!started)
		{
			return;
		}

		frameTimer->stop();
		started = false;
	}

	void ParticleSystem::add(std::shared_ptr<GameObject> ao)
	{
		const auto particleFactory = std::static_pointer_cast<IParticleFactory>(ao);

		if (particleFactory == nullptr)
		{
			Logger::instance()->log("ParticleSystem::Failed to add child, only IParticleFactory is valid");
			return;
		}

		destroyChildren();
		const auto children = getChildren();
		while (children.size() != 0)
		{
			remove(*children.begin());
		}

		this->particleFactory = particleFactory;
		GameObject::add(ao);
	}

	std::shared_ptr<SerializationClient> ParticleSystem::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__particle_system__", hint);

		started = client->serializeBool("started", started);
		particlesPerSecond = client->serializeInt("ppS", particlesPerSecond);
		systemVelocityX = client->serializeDouble("s.xV", systemVelocityX);
		systemVelocityY = client->serializeDouble("s.yV", systemVelocityY);
		isGloballyEmitting = client->serializeBool("globalEmt", isGloballyEmitting);
		isGloballyReceiving = client->serializeBool("globalRcv", isGloballyReceiving);

		return Primitive::doSerialize(hint);
	}
}