#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleBulletTrail : public MT::Particle
	{
	public:
		ParticleBulletTrail(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

