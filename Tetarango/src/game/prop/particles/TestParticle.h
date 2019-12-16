#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class TestParticle : public AWCore::Particle
	{
	public:
		TestParticle(std::shared_ptr<AWCore::TextureContainer> textureContainer);
		void onEnterFrame(double deltaTime);
	};

}

