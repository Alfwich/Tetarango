#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{
	class BlockParticleFactory : public AWCore::IParticleFactory
	{
		AWCore::Color modColor;
	public:
		BlockParticleFactory();

		void setModColor(const AWCore::Color* color);
		std::shared_ptr<AWCore::Particle> makeParticle(AWCore::ParticleCreationBundle& bundle);
	};

	class BlockParticle : public AWCore::Particle
	{
	public:
		BlockParticle(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};
}
