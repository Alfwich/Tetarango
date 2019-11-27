#include "GeneratorBlock.h"
#include "util/RandomGenerator.h"

namespace
{
	MT::RandomGenerator<MTGame::BlockColor> defaultColorGenerator;
}

namespace MTGame
{
	bool GeneratorBlock::hasInit = false;

	void GeneratorBlock::initGenerators()
	{
		hasInit = true;
		defaultColorGenerator.registerCase(BlockColor::Purple, 1);
		defaultColorGenerator.registerCase(BlockColor::Blue, 1);
		defaultColorGenerator.registerCase(BlockColor::Red, 1);
		defaultColorGenerator.registerCase(BlockColor::Yellow, 1);
		defaultColorGenerator.registerCase(BlockColor::Green, 1);
		defaultColorGenerator.registerCase(BlockColor::Orange, 1);
		defaultColorGenerator.registerCase(BlockColor::Cyan, 1);
	}

	GeneratorBlock::GeneratorBlock()
	{
		if (!hasInit)
		{
			initGenerators();
		}
	}

	MT::Color GeneratorBlock::getBlockColor(BlockColor value)
	{
		if (value == BlockColor::None)
		{
			value = defaultColorGenerator.get();
		}

		switch (value)
		{
		case BlockColor::Purple:
			return MT::Color(255, 0, 255);
			break;

		case BlockColor::Blue:
			return MT::Color(0, 0, 255);
			break;

		case BlockColor::Red:
			return MT::Color(255, 0, 0);
			break;

		case BlockColor::Yellow:
			return MT::Color(255, 0, 255);
			break;

		case BlockColor::Green:
			return MT::Color(0, 255, 0);
			break;

		case BlockColor::Orange:
			return MT::Color(255, 192, 0);
			break;

		default:
		case BlockColor::Cyan:
			return MT::Color(0, 255, 255);
			break;
		}
	}

	std::vector<std::shared_ptr<Block>> GeneratorBlock::getTetromino()
	{
		const auto random = defaultColorGenerator.get();
		const auto color = getBlockColor(random);
		std::vector<std::shared_ptr<Block>> result
		{
			std::make_shared<Block>(),
			std::make_shared<Block>(),
			std::make_shared<Block>(),
			std::make_shared<Block>()
		};

		for (const auto block : result)
		{
			block->setColor(color);
		}

		switch (random)
		{
		case BlockColor::Purple:
			result[1]->blockX = -1;
			result[2]->blockX = 1;
			result[3]->blockY = 1;
			break;

		case BlockColor::Blue:
			result[1]->blockY = -1;
			result[2]->blockY = 1;
			result[3]->blockY = 1;
			result[3]->blockX = -1;
			break;

		case BlockColor::Red:
			result[1]->blockX = -1;
			result[2]->blockY = 1;
			result[3]->blockY = 1;
			result[3]->blockX = 1;
			break;

		case BlockColor::Yellow:
			result[1]->blockX = 1;
			result[2]->blockY = 1;
			result[3]->blockY = 1;
			result[3]->blockX = 1;
			break;

		case BlockColor::Green:
			result[1]->blockX = 1;
			result[2]->blockY = 1;
			result[3]->blockY = 1;
			result[3]->blockX = -1;
			break;

		case BlockColor::Orange:
			result[1]->blockY = -1;
			result[2]->blockY = 1;
			result[3]->blockY = 1;
			result[3]->blockX = 1;
			break;

		case BlockColor::Cyan:
			result[1]->blockY = -1;
			result[2]->blockY = 1;
			result[3]->blockY = 2;
			break;
		}

		return result;
	}
}
