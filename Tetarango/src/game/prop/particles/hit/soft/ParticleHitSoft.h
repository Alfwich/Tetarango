#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitSoft : public MT::Particle
	{
	public:
		ParticleHitSoft(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

