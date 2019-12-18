#pragma once

#include <memory>
#include "Particle.h"
#include "engine/GameObject.h"

namespace AW
{
	class ParticleCreationBundle
	{
	public:
		ParticleCreationBundle(Rect rect, double xVelocity, double yVelocity, double rotation, double alpha)
		{
			this->rect = rect;
			this->xVelocity = xVelocity;
			this->yVelocity = yVelocity;
			this->rotation = rotation;
			this->alpha = alpha;
		}
		Rect rect;
		double xVelocity, yVelocity, rotation, alpha;
	};

	class IParticleFactory : public AW::GameObject
	{
	public:
		virtual std::shared_ptr<Particle> makeParticle(ParticleCreationBundle& bundle) = 0;
	};

}

