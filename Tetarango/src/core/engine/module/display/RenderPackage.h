#pragma once

namespace AW
{
	class Renderable;

	class RenderPackage
	{
	public:
		std::shared_ptr<Renderable> obj;
		double alpha = 1.0, rotation = 0.0, zoom = 1.0, xOffset = 0.0, yOffset = 0.0, depth = 1.0;
		Rect computed, world;
		int stencilDepth = 0;
	};
}

#include "ui/renderable/Renderable.h"
