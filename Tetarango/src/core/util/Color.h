#pragma once

#include "NumberHelper.h"

namespace AW
{
	class NormalizedColor
	{
	public:
		const float r, g, b, a;

		NormalizedColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r / 255.f), g(g / 255.f), b(b / 255.f), a(a / 255.f) {}
	};
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

		static Color white() { return Color(255, 255, 255); }
		static Color black() { return Color(0, 0, 0); }
		static Color red() { return Color(255, 0, 0); }
		static Color green() { return Color(0, 255, 0); }
		static Color blue() { return Color(0, 0, 255); }


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

		Color(const Color* other)
		{
			if (other != nullptr)
			{
				r = other->r;
				g = other->g;
				b = other->b;
				a = other->a;
			}
		}


		SDL_Color asSdlColor()
		{
			return SDL_Color
			{
				r, g, b, a
			};
		}

		Color lerp(const Color& other, double p) const
		{
			return Color(
				AW::NumberHelper::lerp<Uint8>(r, other.r, p),
				AW::NumberHelper::lerp<Uint8>(g, other.g, p),
				AW::NumberHelper::lerp<Uint8>(b, other.b, p),
				AW::NumberHelper::lerp<Uint8>(a, other.a, p)
			);
		}

		NormalizedColor asNormalized() const
		{
			return NormalizedColor(r, g, b, a);
		}
	};
}