#pragma once

#include <memory>
#include <Sdl.h>

namespace AW
{
	class Rect
	{
	public:
		double x = 0.0, y = 0.0, w = 0.0, h = 0.0;

		Rect() {};
		Rect(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {};
		Rect(const Rect* other) : x(other->x), y(other->y), w(other->w), h(other->h) {};
		Rect(const Rect& other) : x(other.x), y(other.y), w(other.w), h(other.h) {};

		SDL_Rect asSdlRect();

		double distance(Rect& other)
		{
			return std::sqrt(std::pow((x + w / 2.0) - (other.x + other.w / 2.0), 2) + std::pow((y + h / 2.0) - (other.y + other.h / 2.0), 2));
		}

		double distance(Rect* other)
		{
			return std::sqrt(std::pow((x + w / 2.0) - (other->x + other->w / 2.0), 2) + std::pow((y + h / 2.0) - (other->y + other->h / 2.0), 2));
		}

		bool inRect(double x, double y)
		{
			return x < this->x + this->w && x > this->x &&
				y < this->y + this->h && y > this->y;
		}

		bool intersects(Rect& other, double otherOffset)
		{
			return x  < other.x + other.w + otherOffset
				&& x + w > other.x - otherOffset
				&& y  < other.y + other.h + otherOffset
				&& y + h > other.y - otherOffset;
		}

		bool intersects(Rect* other, double otherOffset)
		{
			return x  < other->x + other->w + otherOffset
				&& x + w > other->x - otherOffset
				&& y  < other->y + other->h + otherOffset
				&& y + h > other->y - otherOffset;
		}

		bool intersects(Rect& other)
		{
			return x  < other.x + other.w
				&& x + w > other.x
				&& y  < other.y + other.h
				&& y + h > other.y;
		}

		bool intersects(Rect* other)
		{
			return x  < other->x + other->w
				&& x + w > other->x
				&& y  < other->y + other->h
				&& y + h > other->y;
		}

		Rect operator * (double scalar);
		Rect operator * (const Rect& other);
		Rect operator + (const Rect& other);
		Rect operator - (const Rect& other);
		bool operator == (const Rect& other);
	};

	class RectI
	{
	public:
		int x = 0, y = 0, w = 0, h = 0;

		RectI();
		RectI(int x, int y, int w, int h);

		SDL_Rect asSdlRect();

		double distance(const Rect& other)
		{
			return std::sqrt(std::pow((x + w / 2.0) - (other.x + other.w / 2.0), 2) + std::pow((y + h / 2.0) - (other.y + other.h / 2.0), 2));
		}

		double distance(const Rect* const other)
		{
			return std::sqrt(std::pow((x + w / 2.0) - (other->x + other->w / 2.0), 2) + std::pow((y + h / 2.0) - (other->y + other->h / 2.0), 2));
		}

		bool inRect(int x, int y)
		{
			return x < this->x + this->w && x > this->x &&
				y < this->y + this->h && y > this->y;
		}

		RectI operator * (int scalar);
		RectI operator * (const RectI& other);
		RectI operator + (const RectI& other);
		RectI operator - (const RectI& other);
		bool operator == (const RectI& other);
	};

}

