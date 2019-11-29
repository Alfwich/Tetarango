#include "NineSlice.h"
#include "Animated.h"

namespace
{
	const auto nineSliceAnimationName = "nine-slice-animation";
	const auto cornerSizeParamName = "corner-size";
}

namespace MT
{
	NineSlice::NineSlice()
	{
		renderType = RenderType::Container;
		enableSerialization<NineSlice>();
	}

	void NineSlice::setCornerSize(unsigned int cornerSize)
	{
		serializationClient->setInt(cornerSizeParamName, cornerSize);
		generateAnimationSet();
	}

	void NineSlice::generateAnimationSet()
	{
		const auto texture = getTexture();
		const auto textureWidth = texture->getWidth();
		const auto textureHeight = texture->getHeight();
		const auto cornerSize = serializationClient->getInt(cornerSizeParamName, 0);

		nineSliceAnimationSet = std::make_shared<MT::AnimationSet>();

		const auto tl = nineSliceAnimationSet->startNewAnimation("top-left");
		tl->addAnimationFrame(0, 0, cornerSize, cornerSize);

		const auto t = nineSliceAnimationSet->startNewAnimation("top");
		t->addAnimationFrame(cornerSize, 0, textureWidth - 2 * cornerSize, cornerSize);

		const auto tr = nineSliceAnimationSet->startNewAnimation("top-right");
		tr->addAnimationFrame(textureWidth - cornerSize, 0, cornerSize, cornerSize);

		const auto l = nineSliceAnimationSet->startNewAnimation("left");
		l->addAnimationFrame(0, cornerSize, cornerSize, textureHeight - 2 * cornerSize);

		const auto c = nineSliceAnimationSet->startNewAnimation("center");
		c->addAnimationFrame(cornerSize, cornerSize, textureWidth - 2 * cornerSize, textureHeight - 2 * cornerSize);

		const auto r = nineSliceAnimationSet->startNewAnimation("right");
		r->addAnimationFrame(textureWidth - cornerSize, cornerSize, cornerSize, textureHeight - 2 * cornerSize);

		const auto bl = nineSliceAnimationSet->startNewAnimation("bottom-left");
		bl->addAnimationFrame(0, textureHeight - cornerSize, cornerSize, cornerSize);

		const auto b = nineSliceAnimationSet->startNewAnimation("bottom");
		b->addAnimationFrame(cornerSize, textureHeight - cornerSize, textureWidth - 2 * cornerSize, cornerSize);

		const auto br = nineSliceAnimationSet->startNewAnimation("bottom-right");
		br->addAnimationFrame(textureWidth - cornerSize, textureHeight - cornerSize, cornerSize, cornerSize);
	}

	void NineSlice::onCreateChildren()
	{
		const auto texture = getTexture();
		const auto cornerSize = serializationClient->getInt(cornerSizeParamName, 0);
		const auto textureWidth = texture->getWidth();
		const auto textureHeight = texture->getHeight();
		const auto cornerBoxWidth = cornerSize;
		const auto cornerBoxHeight = cornerSize;
		const auto centerBoxWidth = getWidth() - 2 * cornerBoxWidth;
		const auto centerBoxHeight = getHeight() - 2 * cornerBoxHeight;

		const auto topLeft = std::make_shared<MT::Animated>();
		topLeft->sizeToAnimation = false;
		topLeft->setAnimationSet(nineSliceAnimationSet);
		topLeft->play("top-left");
		topLeft->setTexture(currentTextureName);
		topLeft->setSizeAndPosition(cornerBoxWidth / 2.0, cornerBoxHeight / 2.0, cornerBoxWidth, cornerBoxHeight);
		add(topLeft);

		const auto top = std::make_shared<MT::Animated>();
		top->sizeToAnimation = false;
		top->setAnimationSet(nineSliceAnimationSet);
		top->play("top");
		top->setTexture(currentTextureName);
		top->setSize(centerBoxWidth, cornerBoxHeight);
		top->toRightOf(topLeft);
		add(top);

		const auto topRight = std::make_shared<MT::Animated>();
		topRight->sizeToAnimation = false;
		topRight->setAnimationSet(nineSliceAnimationSet);
		topRight->play("top-right");
		topRight->setTexture(currentTextureName);
		topRight->setSize(cornerBoxWidth, cornerBoxHeight);
		topRight->toRightOf(top);
		add(topRight);

		const auto left = std::make_shared<MT::Animated>();
		left->sizeToAnimation = false;
		left->setAnimationSet(nineSliceAnimationSet);
		left->play("left");
		left->setTexture(currentTextureName);
		left->setSize(cornerBoxWidth, centerBoxHeight);
		left->toBottomOf(topLeft);
		add(left);

		const auto center = std::make_shared<MT::Animated>();
		center->sizeToAnimation = false;
		center->setAnimationSet(nineSliceAnimationSet);
		center->play("center");
		center->setTexture(currentTextureName);
		center->setSize(centerBoxWidth, centerBoxHeight);
		center->toRightOf(left);
		add(center);

		const auto right = std::make_shared<MT::Animated>();
		right->sizeToAnimation = false;
		right->setAnimationSet(nineSliceAnimationSet);
		right->play("right");
		right->setTexture(currentTextureName);
		right->setSize(cornerBoxWidth, centerBoxHeight);
		right->toRightOf(center);
		add(right);

		const auto bottomLeft = std::make_shared<MT::Animated>();
		bottomLeft->sizeToAnimation = false;
		bottomLeft->setAnimationSet(nineSliceAnimationSet);
		bottomLeft->play("bottom-left");
		bottomLeft->setTexture(currentTextureName);
		bottomLeft->setSize(cornerBoxWidth, cornerBoxHeight);
		bottomLeft->toBottomOf(left);
		add(bottomLeft);

		const auto bottom = std::make_shared<MT::Animated>();
		bottom->sizeToAnimation = false;
		bottom->setAnimationSet(nineSliceAnimationSet);
		bottom->play("bottom");
		bottom->setTexture(currentTextureName);
		bottom->setSize(centerBoxWidth, cornerBoxHeight);
		bottom->toRightOf(bottomLeft);
		add(bottom);

		const auto bottomRight = std::make_shared<MT::Animated>();
		bottomRight->sizeToAnimation = false;
		bottomRight->setAnimationSet(nineSliceAnimationSet);
		bottomRight->play("bottom-right");
		bottomRight->setTexture(currentTextureName);
		bottomRight->setSize(cornerBoxWidth, cornerBoxHeight);
		bottomRight->toRightOf(bottom);
		add(bottomRight);
	}

	void NineSlice::onChildrenHydrated()
	{
		generateAnimationSet();
		for (const auto animated : getChildrenOfType<MT::Animated>())
		{
			animated->setAnimationSet(nineSliceAnimationSet);
		}
	}

	void NineSlice::setTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = texture;
		generateAnimationSet();
	}

	void NineSlice::setTexture(std::string textureName)
	{
		currentTextureName = textureName;
		setTexture(modules->texture->getTexture(textureName));
		generateAnimationSet();
	}
}