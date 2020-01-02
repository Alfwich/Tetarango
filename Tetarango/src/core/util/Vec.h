#pragma once

#include "box2d/b2_math.h"

template <typename T>
class AWVec2
{
public:
	AWVec2(T v) { x = v; y = v; }
	AWVec2(T x, T y) { this->x = x; this->y = y; }

	T x = T(), y = T();

	AWVec2<T> add(const AWVec2<T>& other) const { return AWVec2<T>(x + other.x, y + other.y); }
	AWVec2<T> subtract(const AWVec2<T>& other) const { return AWVec2<T>(x - other.x, y - other.y); }
	AWVec2<T> multiply(const AWVec2<T>& other) const { return AWVec2<T>(x * other.x, y * other.y); }
	AWVec2<T> divide(const AWVec2<T>& other) const { return AWVec2<T>(x / other.x, y / other.y); }

	b2Vec2 asB2() const { return b2Vec2((float)x, (float)y); }
};
