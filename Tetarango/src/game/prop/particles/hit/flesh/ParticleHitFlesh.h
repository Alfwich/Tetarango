#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleHitFlesh : public AWCore::Particle
	{
	public:
		ParticleHitFlesh(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

