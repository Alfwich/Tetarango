#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleSpaceBackground : public AW::Particle
	{
	public:
		ParticleSpaceBackground(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

