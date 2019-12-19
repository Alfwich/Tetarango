#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{
	class BlockParticleFactory : public AW::IParticleFactory
	{
		AW::Color modColor;
	public:
		BlockParticleFactory();

		void setModColor(const AW::Color* color);
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);
	};

	class BlockParticle : public AW::Particle
	{
	public:
		void onEnterFrame(const double& deltaTime);
	};
}
