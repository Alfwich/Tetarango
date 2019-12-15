#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleSpaceBackground : public MT::Particle
	{
	public:
		ParticleSpaceBackground(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

