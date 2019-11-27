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

	Block::Block()
	{
		enableSerialization<Block>();
	}

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

	std::shared_ptr<MT::SerializationClient> Block::doSerialize(MT::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__block__", hint);

		blockX = client->serializeInt("block-x", blockX);
		blockY = client->serializeInt("block-y", blockY);
		hasSettled = client->serializeBool("block-set", hasSettled);

		return Animated::doSerialize(hint);
	}
}
