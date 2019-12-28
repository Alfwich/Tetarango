#include "Rect.h"

namespace AW
{

	SDL_Rect Rect::asSdlRect()
	{
		return SDL_Rect{
			(int)x,
			(int)y,
			(int)w,
			(int)h
		};
	}

	Rect Rect::operator*(double scalar)
	{
		return Rect(
			x * scalar,
			y * scalar,
			w * scalar,
			h * scalar
		);
	}

	Rect Rect::operator*(const Rect & other)
	{
		return Rect(
			x * other.x,
			y * other.y,
			w * other.w,
			h * other.h);
	}

	Rect Rect::operator+(const Rect& other)
	{
		return Rect(
			x + other.x,
			y + other.y,
			w + other.w,
			h + other.h
		);
	}

	Rect Rect::operator-(const Rect& other)
	{
		return Rect(
			x - other.x,
			y - other.y,
			w - other.w,
			h - other.h
		);
	}

	bool Rect::operator==(const Rect& other)
	{
		return x == other.x && y == other.y && w == other.w && h == other.h;
	}

	RectI::RectI()
	{
	}

	RectI::RectI(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	SDL_Rect RectI::asSdlRect()
	{
		return SDL_Rect{
			(int)x,
			(int)y,
			(int)w,
			(int)h
		};
	}

	RectI RectI::operator*(int scalar)
	{
		return RectI(
			x * scalar,
			y * scalar,
			w * scalar,
			h * scalar
		);
	}

	RectI RectI::operator*(const RectI & other)
	{
		return RectI();
	}

	RectI RectI::operator+(const RectI& other)
	{
		return RectI(
			x + other.x,
			y + other.y,
			w + other.w,
			h + other.h
		);
	}

	RectI RectI::operator-(const RectI& other)
	{
		return RectI(
			x - other.x,
			y - other.y,
			w - other.w,
			h - other.h
		);
	}

	bool RectI::operator==(const RectI& other)
	{
		return x == other.x && y == other.y && w == other.w && h == other.h;
	}

}
