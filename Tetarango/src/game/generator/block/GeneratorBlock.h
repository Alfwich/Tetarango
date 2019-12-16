#pragma once

#include "util/Color.h"
#include "prop/block/Block.h"

namespace AWGame
{
	enum class BlockColor
	{
		None,
		Purple,
		Blue,
		Red,
		Yellow,
		Green,
		Orange,
		Cyan
	};

	class GeneratorBlock
	{
		static bool hasInit;
		void initGenerators();

	public:
		GeneratorBlock();

		AWCore::Color getBlockColor(BlockColor value = BlockColor::None);

		std::vector<std::shared_ptr<Block>> getTetromino();
	};

}
