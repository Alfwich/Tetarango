#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace MTGame
{

	class ParticleBulletTrail : public MT::Particle
	{
	public:
		ParticleBulletTrail(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

