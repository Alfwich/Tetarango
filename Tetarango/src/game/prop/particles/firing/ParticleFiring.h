#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleFiring : public AW::Particle
	{
	public:
		ParticleFiring(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(const double& frameTime);
	};

}

