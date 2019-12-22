#pragma once

#include <vector>
#include <list>
#include <map>
#include <random>
#include <queue>
#include "engine/module/time/Timer.h"
#include "ui/renderable/primitive/Primitive.h"
#include "Particle.h"
#include "IParticleFactory.h"

namespace AW
{

	class ParticleSystem : public Primitive
	{
		std::list<std::shared_ptr<Particle>> particles;
		std::shared_ptr<IParticleFactory> particleFactory;
		bool started = false;
		int particlesPerSecond = 1;
		double systemVelocityX = 0.0, systemVelocityY = 0.0;
		std::shared_ptr<Timer> frameTimer;

		bool isGloballyReceiving = false;
		bool isGloballyEmitting = false;
		bool hasCheckedForGlobalSystem = false;;
		std::weak_ptr<ParticleSystem> globalReference;

		void emitParticle(ParticleCreationBundle& bundle, const std::shared_ptr<IParticleFactory>& factory);

		bool findGlobalEmitter();
		std::shared_ptr<ParticleSystem> findClosestGlobalSystem();
		bool checkCandidateAO(std::shared_ptr<GameObject> candiate);
		void acceptParticleFromLocalSystem(std::shared_ptr<Particle> particle);

	public:
		ParticleSystem();

		void onBindShaders();

		void setParticleFactory(std::shared_ptr<IParticleFactory> factory);
		void setParticlesPerSecond(int particlesPerSecond);
		void setSystemVelocity(double xVelocity, double yVelocity);
		void clear();

		void setGlobalEmission(bool value);
		void setGlobalReceiver(bool value);
		void disableGlobalInteractions();

		void onEnterFrame(const double& frameTime);
		void onInitialAttach();
		void onAttach();
		void onDetach();
		void onTimeScopeChanged();

		virtual void preRender(Rect* renderRect, RenderPackage* renderPackage);
		const std::list<std::shared_ptr<Particle>> getParticles();

		void start();
		void emitImmediately(int numberOfParticles);
		void emitImmediatelyWithFactory(int numberOfParticles, const std::shared_ptr<IParticleFactory>& factory);
		void stop();

		virtual void add(std::shared_ptr<GameObject> ao);
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};

}

