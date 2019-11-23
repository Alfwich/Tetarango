#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace MTGame
{

	class ParticleSpaceBackground : public MT::Particle
	{
	public:
		ParticleSpaceBackground(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

