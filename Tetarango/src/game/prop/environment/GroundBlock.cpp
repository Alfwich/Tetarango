#include "GroundBlock.h"

#include "ui/renderable/physic/Chain.h"
#include "ui/renderable/physic/Box.h"
#include "ui/renderable/element/Polygon.h"

namespace
{
	const auto blockIdParam = "gc-id";
	const auto blockDirt = "ground-block-dirt";
	const auto blockDirtGrass = "ground-block-dirt-grass";
	const auto tileSize = 100.0;
}

namespace AWGame
{
	GroundBlock::GroundBlock(double blockSize)
		: blockSize(blockSize)
	{
		GORegister(GroundBlock);
	}

	void GroundBlock::setBlockId(int id)
	{
		serializationClient->setInt(blockIdParam, id);
	}

	int GroundBlock::getBlockId()
	{
		return serializationClient->getInt(blockIdParam, 0);
	}

	void GroundBlock::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/environment/chunk-dirt.png", blockDirt);
		modules->texture->loadTexture("res/image/prop/environment/chunk-dirt-grass.png", blockDirtGrass);
	}

	void GroundBlock::onCreateChildren()
	{
		const auto groundCollider = std::make_shared<AW::Chain>();
		groundCollider->name = "gc";
		groundCollider->visible = false;
		groundCollider->setDynamic(false);
		groundCollider->setWorldPosition(getWorldX(), getWorldY());
		groundCollider->renderPositionMode = AW::RenderPositionMode::NoPositionSelfOnly;
		groundCollider->addScreenPoint(-blockSize / 2.0, 0);
		groundCollider->addScreenPoint(blockSize / 2.0, 0);
		add(groundCollider);

		const auto blocksToMake = blockSize / tileSize;
		for (auto x = -(blocksToMake / 2.0); x < blocksToMake / 2.0; x += 1.0)
		{
			const auto xPos = x * tileSize + tileSize / 2.0;
			for (auto y = 0.0; y < blocksToMake; y += 1.0)
			{
				const auto yPos = y * tileSize + tileSize / 2.0;
				const auto block = std::make_shared<AW::Element>();
				block->setScreenSize(tileSize + 2.0, tileSize + 2.0);
				block->setScreenPosition(xPos - 1.0, yPos - 1.0);

				if (yPos < tileSize)
				{
					block->setTexture(blockDirtGrass);
				}
				else
				{
					block->setTexture(blockDirt);
				}

				const auto shade = (int)AW::NumberHelper::clamp(255 - y * 48.0, 0.0, 255.0);
				block->setColor(shade, shade, shade);

				add(block);
			}
		}
	}
}
