#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleSpaceBackground : public AWCore::Particle
	{
	public:
		ParticleSpaceBackground(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

