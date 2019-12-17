#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "TestParticle.h"

namespace AWGame
{
	class TestParticleFactory : public AW::IParticleFactory
	{
		int r, g, b;
	public:
		TestParticleFactory();
		TestParticleFactory(int r, int g, int b);
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);

		virtual std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
	};

}

