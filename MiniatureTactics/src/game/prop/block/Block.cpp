#include "Block.h"
#include "BlockAnimations.h"

namespace
{
	const MTGame::Blocks config;

	const std::string blockTextureName = "prop-blocks";
	const std::string blockAnimationName = "block-animations";
}

namespace MTGame
{
	bool Block::isLoaded = false;

	void Block::primeBlocks()
	{
		std::make_unique<Block>();
	}

	void Block::loadResources(std::shared_ptr<MT::SystemModuleBundle> modules)
	{
		if (isLoaded)
		{
			return;
		}

		modules->texture->loadTexture(config.blockTexturePath, blockTextureName);
		modules->animation->addAnimationSet(std::make_shared<BlockAnimations>(config), blockAnimationName);

		isLoaded = true;
	}

	void Block::onInitialAttach()
	{
		MT::Animated::onInitialAttach();
		setSize(64, 64);
		setTexture(blockTextureName);
		setAnimationSet(blockAnimationName);
		setCurrentAnimation("block-basic");
	}

	void Block::onCreateChildren()
	{
		MT::Animated::onCreateChildren();
	}
}
