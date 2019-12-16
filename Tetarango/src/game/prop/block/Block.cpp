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
		enableSerialization<Block>();
	}

	void Block::onLoadResources()
	{
		modules->texture->loadTexture("res/game/img/prop/block/blocks.png", blockTextureName);

		auto animationSet = std::make_shared<AWCore::AnimationSet>();
		{
			int fps = 15;
			AWCore::RectI frameSize = {
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
		AWCore::Animated::onInitialAttach();
		setTexture(blockTextureName);
		setAnimationSet(blockAnimationName);
		setCurrentAnimation("default");
	}

	void Block::onCreateChildren()
	{
		AWCore::Animated::onCreateChildren();
	}

	std::shared_ptr<AWCore::SerializationClient> Block::doSerialize(AWCore::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__block__", hint);

		blockX = client->serializeInt("block-x", blockX);
		blockY = client->serializeInt("block-y", blockY);
		hasSettled = client->serializeBool("block-set", hasSettled);

		return Animated::doSerialize(hint);
	}
}
