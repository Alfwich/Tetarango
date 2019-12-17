#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitHard : public AW::Particle
	{
	public:
		ParticleHitHard(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

