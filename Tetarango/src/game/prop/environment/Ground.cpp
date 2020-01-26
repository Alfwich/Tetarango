#include "Ground.h"

namespace
{
	const auto leftBoundParamName = "swt-l-b";
	const auto rightBoundParamName = "swt-r-b";
	const auto chunkExpandFactor = 1;
	const auto chunkExpandDistanceThreshold = 1000.0;
	const auto chunkSize = 5000.0;
}

namespace AWGame
{
	Ground::Ground()
	{
		GORegister(Ground);
	}

	void Ground::onCreateChildren()
	{
		// Center
		const auto ground = std::make_shared<GroundChunk>(chunkSize);
		ground->setScreenPosition(0.0, 0.0);
		add(ground);

		// Prime sides
		for (auto i = -chunkExpandFactor; i < chunkExpandFactor + 1; ++i)
		{
			if (i == 0) continue;

			const auto ground = std::make_shared<GroundChunk>(chunkSize);
			ground->setScreenPosition(i * chunkSize, 0.0);
			add(ground);
		}

		serializationClient->setDouble(leftBoundParamName, -chunkExpandFactor);
		serializationClient->setDouble(rightBoundParamName, chunkExpandFactor);
	}


	void Ground::expandWorldIfNeeded(double worldCenterX)
	{
		const auto leftBound = serializationClient->getDouble(leftBoundParamName);
		const auto rightBound = serializationClient->getDouble(rightBoundParamName);
		const auto leftD = std::abs(leftBound * chunkSize - worldCenterX);
		const auto rightD = std::abs(rightBound * chunkSize - worldCenterX);

		if (leftD < chunkExpandDistanceThreshold)
		{
			for (auto i = leftBound - chunkExpandFactor; i < leftBound; ++i)
			{
				const auto ground = std::make_shared<GroundChunk>(chunkSize);
				ground->setScreenPosition(i * chunkSize, 0.0);
				add(ground);
			}

			serializationClient->setDouble(leftBoundParamName, leftBound - chunkExpandFactor);
		}

		if (rightD < chunkExpandDistanceThreshold)
		{
			for (auto i = rightBound + 1; i < rightBound + chunkExpandFactor + 1; ++i)
			{
				const auto ground = std::make_shared<GroundChunk>(chunkSize);
				ground->setScreenPosition(i * chunkSize, 0.0);
				add(ground);
			}

			serializationClient->setDouble(rightBoundParamName, rightBound + chunkExpandFactor);
		}
	}

	void Ground::updateChunksForNewWorldCenter(double worldCenterX)
	{
		expandWorldIfNeeded(worldCenterX);
	}

}
