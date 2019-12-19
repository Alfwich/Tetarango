#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitFlesh : public AW::Particle
	{
	public:
		ParticleHitFlesh(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(const double& frameTime);
	};

}

