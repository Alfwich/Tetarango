#pragma once

#include <memory>
#include <SDL.h>
#include "util/Rect.h"
#include "engine/module/display/Texture.h"

namespace MT
{
	class Particle
	{
	protected:
		std::shared_ptr<Texture> particleTexture;

	public:
		Rect clip;

		int state = 0, zIndex = 0,
			cModR = 0xff, cModG = 0xff, cModB = 0xff;
		
		bool shouldDecay = true;

		double ogX = 0.0, ogY = 0.0, ogW = 0.0, ogH = 0.0,
			x = 0.0, y = 0.0, w = 0.0, h = 0.0, r = 0.0,
			ogXV = 0.0, ogYV = 0.0, xV = 0.0, yV = 0.0,
			totalLife = 0.0, timeLeft = 0.0,
			alphaMod = 255.0;


		std::shared_ptr<Texture> getTexture();

		virtual void onEnterFrame(double frameTime);
	};
}

