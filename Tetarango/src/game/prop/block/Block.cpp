#include "Block.h"
#include "BlockAnimations.h"

namespace
{
	const AWGame::Blocks config;

	const std::string blockTextureName = "prop-blocks";
	const std::string blockAnimationName = "block-animations";
}

namespace AWGame
{
	Block::Block()
	{
		enableSerialization<Block>();
	}

	void Block::onLoadResources()
	{
		modules->texture->loadTexture(config.blockTexturePath, blockTextureName);
		modules->animation->addAnimationSet(std::make_shared<BlockAnimations>(config), blockAnimationName);
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
