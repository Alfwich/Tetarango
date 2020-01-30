#include "Ground.h"

namespace
{
	const auto leftBoundParamName = "swt-l-b";
	const auto rightBoundParamName = "swt-r-b";
	const auto blockExpandFactor = 2;
	const auto blockVisibleRange = 3;
	const auto blockExpandDistanceThreshold = 1000.0;
	const auto blockSize = 1000.0;
}

namespace AWGame
{
	Ground::Ground()
	{
		GORegister(Ground);
	}

	void Ground::createChunk(int id)
	{
		const auto xOffset = id * blockSize;
		const auto block = std::make_shared<GroundBlock>(blockSize);
		block->setScreenPosition(xOffset, AW::NumberHelper::random(-25.0, 25.0));
		block->setBlockId(id);
		blockMap[id] = block;
		add(block);
	}

	void Ground::updateVisibleChunks(double worldCenterX)
	{
		const auto currentBlockId = (int)std::floor((worldCenterX - blockSize / 2.0) / blockSize) + 1;
		if (currentBlockId != currentVisibleBlock)
		{
			for (const auto blockIdToBlock : blockMap)
			{
				blockIdToBlock.second->visible = (std::abs(blockIdToBlock.first - currentBlockId) < blockVisibleRange);
			}
		}
	}

	void Ground::onCreateChildren()
	{
		// Prime 
		for (auto i = -blockExpandFactor; i < blockExpandFactor + 1; ++i)
		{
			createChunk(i);
		}

		serializationClient->setDouble(leftBoundParamName, -blockExpandFactor);
		serializationClient->setDouble(rightBoundParamName, blockExpandFactor);
	}

	void Ground::onChildrenHydrated()
	{
		for (const auto groundBlock : getChildrenOfType<GroundBlock>())
		{
			blockMap[groundBlock->getBlockId()] = groundBlock;
		}
	}

	void Ground::expandWorldIfNeeded(double worldCenterX)
	{
		const auto leftBound = serializationClient->getDouble(leftBoundParamName);
		const auto rightBound = serializationClient->getDouble(rightBoundParamName);
		const auto leftD = std::abs(leftBound * blockSize - worldCenterX);
		const auto rightD = std::abs(rightBound * blockSize - worldCenterX);

		if (leftD < blockExpandDistanceThreshold)
		{
			for (auto i = leftBound - blockExpandFactor; i < leftBound; ++i)
			{
				createChunk(i);
			}

			serializationClient->setDouble(leftBoundParamName, leftBound - blockExpandFactor);
		}

		if (rightD < blockExpandDistanceThreshold)
		{
			for (auto i = rightBound + 1; i < rightBound + blockExpandFactor + 1; ++i)
			{
				createChunk(i);
			}

			serializationClient->setDouble(rightBoundParamName, rightBound + blockExpandFactor);
		}
	}

	void Ground::updateChunksForNewWorldCenter(double worldCenterX)
	{
		expandWorldIfNeeded(worldCenterX);
		updateVisibleChunks(worldCenterX);
	}

}
