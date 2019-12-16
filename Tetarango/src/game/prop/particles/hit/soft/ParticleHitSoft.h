#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitSoft : public AWCore::Particle
	{
	public:
		ParticleHitSoft(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

