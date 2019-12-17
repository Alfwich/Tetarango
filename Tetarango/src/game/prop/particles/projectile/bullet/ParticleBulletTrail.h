#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleBulletTrail : public AW::Particle
	{
	public:
		ParticleBulletTrail(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

