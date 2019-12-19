#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleSpaceBackground : public AW::Particle
	{
	public:
		void onEnterFrame(const double& deltaTime);
	};

}

