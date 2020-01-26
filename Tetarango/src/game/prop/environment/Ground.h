#pragma once

#include "ui/renderable/container/Container.h"
#include "GroundChunk.h"

namespace AWGame
{
	class Ground : public AW::Container
	{
		void expandWorldIfNeeded(double worldCenterX);

	public:
		Ground();

		void onCreateChildren() override;

		void updateChunksForNewWorldCenter(double worldCenterX);
	};
}
