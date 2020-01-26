#pragma once

#include "ui/renderable/container/Container.h"

namespace AWGame
{
	class GroundChunk : public AW::Container
	{
		double chunkSize;
	public:
		GroundChunk(double chunkSize = 2000.0);

		void onLoadResources() override;

		void onCreateChildren() override;
	};
}
