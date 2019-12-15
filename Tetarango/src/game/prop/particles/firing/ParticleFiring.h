#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleFiring : public MT::Particle
	{
	public:
		ParticleFiring(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

