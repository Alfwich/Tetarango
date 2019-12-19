#include "Block.h"

namespace
{
	const std::string blockTextureName = "prop-blocks";
	const std::string blockAnimationName = "block-animations";
	const auto blockEffectIntensity = 10.0;
	const auto blockEffectShift = -5.0;
}

namespace AWGame
{
	Block::Block()
	{
		setSize(32, 32);
		registerSerialization<Block>();
	}

	void Block::onTransitionAnimationFrame(double position)
	{
		if (position < 0.75)
		{
			fragmentShader->setFloatIUParam("effectAmount", blockEffectShift);
		}
		else if (position < 0.875)
		{
			fragmentShader->setFloatIUParam("effectAmount", blockEffectShift + ((position - 0.75) * 8.0) * blockEffectIntensity);
		}
		else if (position < 1.0)
		{
			fragmentShader->setFloatIUParam("effectAmount", blockEffectShift + blockEffectIntensity - (blockEffectIntensity * ((position - 0.875) * 8.0)));
		}
		else
		{
			fragmentShader->setFloatIUParam("effectAmount", blockEffectShift);
		}
	}

	void Block::onLoadResources()
	{
		modules->texture->loadTexture("res/img/prop/block/blocks.png", blockTextureName);
		modules->shader->loadShader("res/shader/fragment/block.glsl", "fragment-block");

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

	void Block::onBindShaders()
	{
		Animated::onBindShaders();

		fragmentShader = modules->shader->getShader("fragment-block");
	}

	void Block::onInitialAttach()
	{
		AW::Animated::onInitialAttach();

		setTexture(blockTextureName);
		setAnimationSet(blockAnimationName);
		setCurrentAnimation("default");

		blockTransition = modules->animation->createTransition();
		blockTransition->listener = shared_from_this();
		blockTransition->setLooping(true);
		blockTransition->startTargetlessTransition(AW::NumberHelper::random(1000.0, 1750.0));
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
