#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{
	class BlockParticleFactory : public MT::IParticleFactory
	{
		MT::Color modColor;
	public:
		BlockParticleFactory();

		void setModColor(const MT::Color* color);
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

	class BlockParticle : public MT::Particle
	{
	public:
		BlockParticle(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};
}
