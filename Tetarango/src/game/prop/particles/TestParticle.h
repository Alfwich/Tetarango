#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace MTGame
{

	class TestParticle : public MT::Particle
	{
	public:
		TestParticle(std::shared_ptr<MT::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

