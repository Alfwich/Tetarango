#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "TestParticle.h"

namespace AWGame
{
	class TestParticleFactory : public MT::IParticleFactory
	{
		int r, g, b;
	public:
		TestParticleFactory();
		TestParticleFactory(int r, int g, int b);
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);

		virtual std::shared_ptr<MT::SerializationClient> doSerialize(MT::SerializationHint hint);
	};

}

