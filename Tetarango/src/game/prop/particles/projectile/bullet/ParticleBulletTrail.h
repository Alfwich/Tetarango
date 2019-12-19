#pragma once

#include "ui/renderable/primitive/particle/Particle.h"

namespace AWGame
{

	class ParticleBulletTrail : public AW::Particle
	{
	public:
		void onEnterFrame(const double& deltaTime);
	};

}

