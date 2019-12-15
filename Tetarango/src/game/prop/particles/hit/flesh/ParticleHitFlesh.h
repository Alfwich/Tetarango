#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitFlesh : public MT::Particle
	{
	public:
		ParticleHitFlesh(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

