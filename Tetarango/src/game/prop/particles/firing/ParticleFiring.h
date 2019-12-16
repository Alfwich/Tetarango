#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleFiring : public AWCore::Particle
	{
	public:
		ParticleFiring(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double frameTime);
	};

}

