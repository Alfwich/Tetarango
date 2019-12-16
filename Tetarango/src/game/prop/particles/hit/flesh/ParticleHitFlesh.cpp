#include "ParticleHitFlesh.h"

namespace AWGame
{

	ParticleHitFlesh::ParticleHitFlesh(std::shared_ptr<AWCore::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("particle-hit-flesh");
	}

	void ParticleHitFlesh::onEnterFrame(double frameTime)
	{
		switch (state)
		{
		case 0:
		{
			x += ogXV * frameTime;
			y += ogYV * frameTime;

			double position = timeLeft / totalLife;

			w = ogW * position;
			h = ogH * position;
		}
		break;
		case 1:
		{
			double position = timeLeft / totalLife;

			if (position < 0.05)
			{
				w = ogW * (position * 20.0);
				h = ogH * (position * 20.0);
			}
			else
			{
				w = ogW;
				h = ogH;
			}
		}
		break;
		}
	}

}
