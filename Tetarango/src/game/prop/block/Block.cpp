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
		GORegister(Block);
	}

	double Block::addEnergy(double amount)
	{
		energy = energy + amount;

		double r = 0.0;
		if (energy > 1.0)
		{
			r = energy - 1.0;
			energy = 1.0;
		}

		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("blockCenterFill", energy);
		}

		return r;
	}

	double Block::removeEnergy(double amount)
	{
		if (energy == 0.0)
		{
			return 0.0;
		}

		if (energy < amount)
		{
			const auto a = amount - energy;
			energy = 0.0;
			return a;
		}

		energy -= amount;

		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("blockCenterFill", energy);
		}

		return amount;
	}

	void Block::setEnergy(double amount)
	{
		if (amount == energy)
		{
			return;
		}

		energy = AW::NumberHelper::clamp(amount, 0.0, 1.0);

		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("blockCenterFill", energy);
		}
	}

	double Block::getEnergy()
	{
		return energy;
	}

	void Block::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/block/blocks.png", blockTextureName);
		modules->shader->loadShader("res/shader/fragment/block-pulsate.glsl", "f-block-pulsate");
		modules->shader->loadShader("res/shader/fragment/block-fill.glsl", "f-block-fill");
		modules->shader->loadShader("res/shader/fragment/block.glsl", "f-block");
		modules->shader->registerShaderComposition({ "f-clip-texture", "f-texture", "f-color", "f-block", "f-block-fill", "f-scanline-retro" }, "block");
	}

	void Block::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "block" }, true);
		fragmentShader->setFloatIUParam("clipX", 32.0);
		fragmentShader->setFloatIUParam("clipY", 0.0);
		fragmentShader->setFloatIUParam("clipWidth", 64.0);
		fragmentShader->setFloatIUParam("clipHeight", 64.0);

		fragmentShader->setFloatIUParam("blockBorderSize", 2.0);
		fragmentShader->setFloatIUParam("blockEffect", 0.5);
		fragmentShader->setFloatIUParam("blockEffectP", 0.25);
		fragmentShader->setFloatIUParam("blockEffectG", 0.4);
		fragmentShader->setFloatIUParam("blockCenterFill", energy);

		fragmentShader->setFloatIUParam("fScanlineRetroAmount", 0.25);
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

		setEnergy(client->serializeDouble("energy", energy));

		return Element::doSerialize(hint);
	}
}
