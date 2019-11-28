#include "NineSlice.h"
#include "Animated.h"

namespace
{
	const auto nineSliceAnimationName = "nine-slice-animation";
}

namespace MT
{
	NineSlice::NineSlice()
	{
		renderType = RenderType::Container;
		enableSerialization<NineSlice>();
	}

	void NineSlice::onInitialAttach()
	{
		if (!modules->animation->hasAnimationSet(nineSliceAnimationName))
		{
			const auto cornerSize = 20.0;
			const auto textureWidth = 64.0;
			const auto textureHeight = 64.0;

			const auto nineSliceAnimationSet = std::make_shared<MT::AnimationSet>();

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

			modules->animation->addAnimationSet(nineSliceAnimationSet, nineSliceAnimationName);
		}
	}

	void NineSlice::onCreateChildren()
	{
		const auto cornerSize = 20.0;
		const auto textureWidth = 64.0;
		const auto textureHeight = 64.0;
		const auto cornerBoxWidth = cornerSize;//std::floor((cornerSize / textureWidth) * getWidth());
		const auto cornerBoxHeight = cornerSize;//std::floor((cornerSize / textureHeight) * getHeight());
		const auto centerBoxWidth = getWidth() - 2 * cornerBoxWidth;
		const auto centerBoxHeight = getHeight() - 2 * cornerBoxHeight;

		const auto topLeft = std::make_shared<MT::Animated>();
		topLeft->sizeToAnimation = false;
		topLeft->setAnimationSet(nineSliceAnimationName);
		topLeft->play("top-left");
		topLeft->setTexture(currentTextureName);
		topLeft->setSizeAndPosition(cornerBoxWidth / 2.0, cornerBoxHeight / 2.0, cornerBoxWidth, cornerBoxHeight);
		add(topLeft);

		const auto top = std::make_shared<MT::Animated>();
		top->sizeToAnimation = false;
		top->setAnimationSet(nineSliceAnimationName);
		top->play("top");
		top->setTexture(currentTextureName);
		top->setSize(centerBoxWidth, cornerBoxHeight);
		top->toRightOf(topLeft);
		add(top);

		const auto topRight = std::make_shared<MT::Animated>();
		topRight->sizeToAnimation = false;
		topRight->setAnimationSet(nineSliceAnimationName);
		topRight->play("top-right");
		topRight->setTexture(currentTextureName);
		topRight->setSize(cornerBoxWidth, cornerBoxHeight);
		topRight->toRightOf(top);
		add(topRight);

		const auto left = std::make_shared<MT::Animated>();
		left->sizeToAnimation = false;
		left->setAnimationSet(nineSliceAnimationName);
		left->play("left");
		left->setTexture(currentTextureName);
		left->setSize(cornerBoxWidth, centerBoxHeight);
		left->toBottomOf(topLeft);
		add(left);

		const auto center = std::make_shared<MT::Animated>();
		center->sizeToAnimation = false;
		center->setAnimationSet(nineSliceAnimationName);
		center->play("center");
		center->setTexture(currentTextureName);
		center->setSize(centerBoxWidth, centerBoxHeight);
		center->toRightOf(left);
		add(center);

		const auto right = std::make_shared<MT::Animated>();
		right->sizeToAnimation = false;
		right->setAnimationSet(nineSliceAnimationName);
		right->play("right");
		right->setTexture(currentTextureName);
		right->setSize(cornerBoxWidth, centerBoxHeight);
		right->toRightOf(center);
		add(right);

		const auto bottomLeft = std::make_shared<MT::Animated>();
		bottomLeft->sizeToAnimation = false;
		bottomLeft->setAnimationSet(nineSliceAnimationName);
		bottomLeft->play("bottom-left");
		bottomLeft->setTexture(currentTextureName);
		bottomLeft->setSize(cornerBoxWidth, cornerBoxHeight);
		bottomLeft->toBottomOf(left);
		add(bottomLeft);

		const auto bottom = std::make_shared<MT::Animated>();
		bottom->sizeToAnimation = false;
		bottom->setAnimationSet(nineSliceAnimationName);
		bottom->play("bottom");
		bottom->setTexture(currentTextureName);
		bottom->setSize(centerBoxWidth, cornerBoxHeight);
		bottom->toRightOf(bottomLeft);
		add(bottom);

		const auto bottomRight = std::make_shared<MT::Animated>();
		bottomRight->sizeToAnimation = false;
		bottomRight->setAnimationSet(nineSliceAnimationName);
		bottomRight->play("bottom-right");
		bottomRight->setTexture(currentTextureName);
		bottomRight->setSize(cornerBoxWidth, cornerBoxHeight);
		bottomRight->toRightOf(bottom);
		add(bottomRight);
	}

	void NineSlice::onChildrenHydrated()
	{
		std::cout << "Hello World!" << std::endl;
	}
}