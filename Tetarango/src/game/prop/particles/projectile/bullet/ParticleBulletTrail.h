#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleBulletTrail : public AWCore::Particle
	{
	public:
		ParticleBulletTrail(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

