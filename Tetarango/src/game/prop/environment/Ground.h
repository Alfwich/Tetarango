#pragma once

#include "ui/renderable/container/Container.h"
#include "GroundBlock.h"

namespace AWGame
{
	class Ground : public AW::Container
	{
		int currentVisibleBlock = INT_MAX;
		std::unordered_map<int, std::shared_ptr<GroundBlock>> blockMap;

		void expandWorldIfNeeded(double worldCenterX);
		void createChunk(int id);
		void updateVisibleChunks(double worldCenterX);

	public:
		Ground();

		void onCreateChildren() override;

		void onChildrenHydrated() override;

		void updateChunksForNewWorldCenter(double worldCenterX);
	};
}
