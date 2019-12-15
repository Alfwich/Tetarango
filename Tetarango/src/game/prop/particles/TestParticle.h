#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class TestParticle : public MT::Particle
	{
	public:
		TestParticle(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

