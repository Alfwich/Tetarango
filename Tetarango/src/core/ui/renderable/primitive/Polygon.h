#pragma once

#include "ui/renderable/primitive/Primitive.h"

namespace AW
{
	class Polygon : public Primitive
	{
		std::vector<LM::vec2> points;
	public:
		Polygon();
	};
}
