#pragma once

#include "util/QuadMap.h"

namespace AW
{

	class RenderPackage
	{
	public:
		double alpha = 1.0, rotation = 0.0, zoom = 1.0, cameraX = 0.0, cameraY = 0.0, xOffset = 0.0, yOffset = 0.0, depth = 1.0;
		int stencilDepth = 0;
	};
}
