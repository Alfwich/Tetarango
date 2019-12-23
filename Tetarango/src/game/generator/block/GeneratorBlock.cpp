#include "GeneratorBlock.h"
#include "util/RandomGenerator.h"

namespace
{
	AW::RandomGenerator<AWGame::BlockColor> defaultColorGenerator;
}

namespace AWGame
{
	bool GeneratorBlock::hasInit = false;

	void GeneratorBlock::initGenerators()
	{
		hasInit = true;
		defaultColorGenerator.registerCase(BlockColor::Purple);
		defaultColorGenerator.registerCase(BlockColor::Blue);
		defaultColorGenerator.registerCase(BlockColor::Red);
		defaultColorGenerator.registerCase(BlockColor::Yellow);
		defaultColorGenerator.registerCase(BlockColor::Green);
		defaultColorGenerator.registerCase(BlockColor::Orange);
		defaultColorGenerator.registerCase(BlockColor::Cyan, 100000);
	}

	GeneratorBlock::GeneratorBlock()
	{
		if (!hasInit)
		{
			initGenerators();
		}
	}

	AW::Color GeneratorBlock::getBlockColor(BlockColor value)
	{
		if (value == BlockColor::None)
		{
			value = defaultColorGenerator.get();
		}

		switch (value)
		{
		case BlockColor::Purple:
			return AW::Color(0xa001efff);
			break;

		case BlockColor::Blue:
			return AW::Color(0x0001f2ff);
			break;

		case BlockColor::Red:
			return AW::Color(0xf00100ff);
			break;

		case BlockColor::Yellow:
			return AW::Color(0xf0f001ff);
			break;

		case BlockColor::Green:
			return AW::Color(0x00f000ff);
			break;

		case BlockColor::Orange:
			return AW::Color(0xefa000ff);
			break;

		default:
		case BlockColor::Cyan:
			return AW::Color(0x01f0f1ff);
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
