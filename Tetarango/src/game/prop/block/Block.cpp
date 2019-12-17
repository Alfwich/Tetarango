#include "Block.h"

namespace
{
	const std::string blockTextureName = "prop-blocks";
	const std::string blockAnimationName = "block-animations";
}

namespace AWGame
{
	Block::Block()
	{
		setSize(32, 32);
		registerSerialization<Block>();
	}

	void Block::onLoadResources()
	{
		modules->texture->loadTexture("res/game/img/prop/block/blocks.png", blockTextureName);

		auto animationSet = std::make_shared<AW::AnimationSet>();
		{
			int fps = 15;
			AW::RectI frameSize = {
				0,
				0,
				32,
				32
			};

			{
				auto anim = animationSet->startNewAnimation("default");
				anim->setFps(fps);
				anim->addGeneralFrames(0, 0, frameSize.w, frameSize.h, 1);
			}
		}

		modules->animation->addAnimationSet(animationSet, blockAnimationName);
	}

	void Block::onInitialAttach()
	{
		AW::Animated::onInitialAttach();
		setTexture(blockTextureName);
		setAnimationSet(blockAnimationName);
		setCurrentAnimation("default");
	}

	void Block::onCreateChildren()
	{
		AW::Animated::onCreateChildren();
	}

	std::shared_ptr<AW::SerializationClient> Block::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__block__", hint);

		blockX = client->serializeInt("block-x", blockX);
		blockY = client->serializeInt("block-y", blockY);
		hasSettled = client->serializeBool("block-set", hasSettled);

		return Animated::doSerialize(hint);
	}
}
