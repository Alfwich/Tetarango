#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class TestParticle : public AW::Particle
	{
	public:
		TestParticle(std::shared_ptr<AW::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

