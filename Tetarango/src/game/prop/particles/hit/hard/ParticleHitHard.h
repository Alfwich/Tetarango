#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitHard : public AWCore::Particle
	{
	public:
		ParticleHitHard(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

