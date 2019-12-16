#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "TestParticle.h"

namespace AWGame
{
	class TestParticleFactory : public AWCore::IParticleFactory
	{
		int r, g, b;
	public:
		TestParticleFactory();
		TestParticleFactory(int r, int g, int b);
		std::shared_ptr<AWCore::Particle> makeParticle(AWCore::ParticleCreationBundle& bundle);

		virtual std::shared_ptr<AWCore::SerializationClient> doSerialize(AWCore::SerializationHint hint);
	};

}

