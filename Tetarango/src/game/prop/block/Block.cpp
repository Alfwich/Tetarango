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

	void Block::addEnergy(double amount)
	{
		energy += amount;

		if (fragmentShader != nullptr)
		{
		}
	}

	double Block::getEnergy()
	{
		return energy;
	}

	void Block::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/block/blocks.png", blockTextureName);

		modules->shader->registerShaderComposition({ "f-clip", "f-texture", "f-color", "f-pulsate", "f-negate", "f-alpha" }, "block");
	}

	void Block::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "block" }, true);
		fragmentShader->setFloatIUParam("clipX", 0.0);
		fragmentShader->setFloatIUParam("clipY", 0.0);
		fragmentShader->setFloatIUParam("clipWidth", 32.0);
		fragmentShader->setFloatIUParam("clipHeight", 32.0);

		fragmentShader->setFloatIUParam("pulsateAmount", AW::NumberHelper::random(0.0, 0.3));
		fragmentShader->setFloatIUParam("blurAmount", AW::NumberHelper::random());
		fragmentShader->setFloatIUParam("alpha", 1.0);
	}

	void Block::onInitialAttach()
	{
		setTexture(blockTextureName);
	}

	std::shared_ptr<AW::SerializationClient> Block::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__block__", hint);

		blockX = client->serializeInt("block-x", blockX);
		blockY = client->serializeInt("block-y", blockY);
		hasSettled = client->serializeBool("block-set", hasSettled);

		return Element::doSerialize(hint);
	}
}
