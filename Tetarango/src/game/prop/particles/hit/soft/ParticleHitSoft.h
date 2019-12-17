#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitSoft : public AW::Particle
	{
	public:
		ParticleHitSoft(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

