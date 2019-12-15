#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitHard : public MT::Particle
	{
	public:
		ParticleHitHard(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

