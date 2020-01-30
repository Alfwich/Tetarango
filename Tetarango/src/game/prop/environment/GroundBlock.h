#pragma once

#include "ui/renderable/container/Container.h"

namespace AWGame
{
	class GroundBlock : public AW::Container
	{
		double blockSize;

	public:
		GroundBlock(double blockSize = 1000.0);

		void setBlockId(int id);
		int getBlockId();

		void onLoadResources() override;

		void onCreateChildren() override;
	};
}
