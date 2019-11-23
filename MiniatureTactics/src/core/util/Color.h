#pragma once

#include "NumberHelper.h"
#include "SDL.h"

namespace MT
{
	class Color
	{
	public:
		static Color random()
		{
			return Color(
				NumberHelper::randomInt(0, 255),
				NumberHelper::randomInt(0, 255),
				NumberHelper::randomInt(0, 255)
			);
		}

		Uint8 r = 0, g = 0, b = 0, a = 0xff;

		Color() {};

		Color(int r, int g, int b, int a = 0xff)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color(int color, int alpha = 0xff)
		{
			r = (color & 0xFF000000) >> 24;
			g = (color & 0x00FF0000) >> 16;
			b = (color & 0x0000FF00) >> 8;
			this->a = alpha;
		}

		SDL_Color asSdlColor()
		{
			return SDL_Color
			{
				r, g, b, a
			};
		}

		Color lerp(const Color& other, double p)
		{
			return Color(
				MT::NumberHelper::lerp<Uint8>(r, other.r, p),
				MT::NumberHelper::lerp<Uint8>(g, other.g, p),
				MT::NumberHelper::lerp<Uint8>(b, other.b, p),
				MT::NumberHelper::lerp<Uint8>(a, other.a, p)
			);
		}

	};
}