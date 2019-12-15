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
		generateNineSliceAnimationSet();
	}

	void NineSlice::generateNineSliceAnimationSet()
	{
		if (texture == nullptr)
		{
			return;
		}

		if (nineSliceAnimationSet == nullptr)
		{
			nineSliceAnimationSet = std::make_shared<MT::AnimationSet>();
		}

		const auto cornerSize = serializationClient->getInt(cornerSizeParamName, 0);
		if (animationSet == nullptr)
		{
			const auto textureWidth = texture->getWidth();
			const auto textureHeight = texture->getHeight();

			pushAnimation("", cornerSize, 0, 0, textureWidth, textureHeight);
		}
		else
		{
			for (const auto animation : animationSet->getAnimations())
			{
				const auto rect = animation.second->getFrameRect(0);
				pushAnimation(animation.first, cornerSize, rect->x, rect->y, rect->w, rect->h);
			}
		}

		for (const auto animated : getChildrenOfType<MT::Animated>())
		{
			animated->setTexture(texture);
			animated->setAnimationSet(nineSliceAnimationSet);
		}
	}

	void NineSlice::pushAnimation(std::string prefix, int cornerSize, int x, int y, int width, int height)
	{

		if (nineSliceAnimationSet == nullptr)
		{
			Logger::instance()->log("NineSlice > pushAnimation: Attempted to create animation before animation set was created");
			return;
		}

		const auto tl = nineSliceAnimationSet->startNewAnimation(prefix + "top-left");
		tl->addAnimationFrame(x, y, cornerSize, cornerSize);

		const auto t = nineSliceAnimationSet->startNewAnimation(prefix + "top");
		t->addAnimationFrame(x + cornerSize, y, width - 2 * cornerSize, cornerSize);

		const auto tr = nineSliceAnimationSet->startNewAnimation(prefix + "top-right");
		tr->addAnimationFrame(x + width - cornerSize, y, cornerSize, cornerSize);

		const auto l = nineSliceAnimationSet->startNewAnimation(prefix + "left");
		l->addAnimationFrame(x, y + cornerSize, cornerSize, height - 2 * cornerSize);

		const auto c = nineSliceAnimationSet->startNewAnimation(prefix + "center");
		c->addAnimationFrame(x + cornerSize, y + cornerSize, width - 2 * cornerSize, height - 2 * cornerSize);

		const auto r = nineSliceAnimationSet->startNewAnimation(prefix + "right");
		r->addAnimationFrame(x + width - cornerSize, y + cornerSize, cornerSize, height - 2 * cornerSize);

		const auto bl = nineSliceAnimationSet->startNewAnimation(prefix + "bottom-left");
		bl->addAnimationFrame(x, y + height - cornerSize, cornerSize, cornerSize);

		const auto b = nineSliceAnimationSet->startNewAnimation(prefix + "bottom");
		b->addAnimationFrame(x + cornerSize, y + height - cornerSize, width - 2 * cornerSize, cornerSize);

		const auto br = nineSliceAnimationSet->startNewAnimation(prefix + "bottom-right");
		br->addAnimationFrame(x + width - cornerSize, y + height - cornerSize, cornerSize, cornerSize);
	}

	void NineSlice::onCreateChildren()
	{
		topLeft = std::make_shared<MT::Animated>();
		topLeft->name = "tl";
		topLeft->sizeToAnimation = false;
		topLeft->setAnimationSet(nineSliceAnimationSet);
		topLeft->play("top-left");
		topLeft->setTexture(texture);
		add(topLeft);

		top = std::make_shared<MT::Animated>();
		top->name = "t";
		top->sizeToAnimation = false;
		top->setAnimationSet(nineSliceAnimationSet);
		top->play("top");
		top->setTexture(texture);
		add(top);

		topRight = std::make_shared<MT::Animated>();
		topRight->name = "tr";
		topRight->sizeToAnimation = false;
		topRight->setAnimationSet(nineSliceAnimationSet);
		topRight->play("top-right");
		topRight->setTexture(texture);
		add(topRight);

		left = std::make_shared<MT::Animated>();
		left->name = "l";
		left->sizeToAnimation = false;
		left->setAnimationSet(nineSliceAnimationSet);
		left->play("left");
		left->setTexture(texture);
		add(left);

		center = std::make_shared<MT::Animated>();
		center->name = "c";
		center->sizeToAnimation = false;
		center->setAnimationSet(nineSliceAnimationSet);
		center->play("center");
		center->setTexture(texture);
		add(center);

		right = std::make_shared<MT::Animated>();
		right->name = "r";
		right->sizeToAnimation = false;
		right->setAnimationSet(nineSliceAnimationSet);
		right->play("right");
		right->setTexture(texture);
		add(right);

		bottomLeft = std::make_shared<MT::Animated>();
		bottomLeft->name = "bl";
		bottomLeft->sizeToAnimation = false;
		bottomLeft->setAnimationSet(nineSliceAnimationSet);
		bottomLeft->play("bottom-left");
		bottomLeft->setTexture(texture);
		add(bottomLeft);

		bottom = std::make_shared<MT::Animated>();
		bottom->name = "b";
		bottom->sizeToAnimation = false;
		bottom->setAnimationSet(nineSliceAnimationSet);
		bottom->play("bottom");
		bottom->setTexture(texture);
		add(bottom);

		bottomRight = std::make_shared<MT::Animated>();
		bottomRight->name = "br";
		bottomRight->sizeToAnimation = false;
		bottomRight->setAnimationSet(nineSliceAnimationSet);
		bottomRight->play("bottom-right");
		bottomRight->setTexture(texture);
		add(bottomRight);
	}

	void NineSlice::onChildrenHydrated()
	{
		topRight = findChildWithName<MT::Animated>("tr");
		top = findChildWithName<MT::Animated>("t");
		topLeft = findChildWithName<MT::Animated>("tl");

		right = findChildWithName<MT::Animated>("r");
		center = findChildWithName<MT::Animated>("c");
		left = findChildWithName<MT::Animated>("l");

		bottomRight = findChildWithName<MT::Animated>("br");
		bottom = findChildWithName<MT::Animated>("b");
		bottomLeft = findChildWithName<MT::Animated>("bl");

		generateNineSliceAnimationSet();
	}

	void NineSlice::onLayoutChildren()
	{
		const auto cornerSize = serializationClient->getInt(cornerSizeParamName, 0);
		const auto cornerBoxWidth = cornerSize;
		const auto cornerBoxHeight = cornerSize;
		const auto centerBoxWidth = getWidth() - 2 * cornerBoxWidth;
		const auto centerBoxHeight = getHeight() - 2 * cornerBoxHeight;

		topLeft->setSizeAndPosition(cornerBoxWidth / 2.0, cornerBoxHeight / 2.0, cornerBoxWidth, cornerBoxHeight);

		top->setSize(centerBoxWidth, cornerBoxHeight);
		top->toRightOf(topLeft);

		topRight->setSize(cornerBoxWidth, cornerBoxHeight);
		topRight->toRightOf(top);

		left->setSize(cornerBoxWidth, centerBoxHeight);
		left->toBottomOf(topLeft);

		center->setSize(centerBoxWidth, centerBoxHeight);
		center->toRightOf(left);

		right->setSize(cornerBoxWidth, centerBoxHeight);
		right->toRightOf(center);

		bottomLeft->setSize(cornerBoxWidth, cornerBoxHeight);
		bottomLeft->toBottomOf(left);

		bottom->setSize(centerBoxWidth, cornerBoxHeight);
		bottom->toRightOf(bottomLeft);

		bottomRight->setSize(cornerBoxWidth, cornerBoxHeight);
		bottomRight->toRightOf(bottom);
	}

	void NineSlice::play(std::string animationName)
	{
		for (const auto animated : getChildrenOfType<MT::Animated>())
		{
			animated->setAnimationPrefix(animationName);
			animated->play(animated->getCurrentAnimationName());
		}
	}

	void NineSlice::setAnimationSet(std::string animationSetName)
	{
		currentAnimationSetName = animationSetName;
		setAnimationSet(modules->animation->getAnimationSet(animationSetName));
	}

	void NineSlice::setAnimationSet(std::shared_ptr<AnimationSet> animationSet)
	{
		this->animationSet = animationSet;
		generateNineSliceAnimationSet();
	}

	void NineSlice::setTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = texture;
		generateNineSliceAnimationSet();
	}

	void NineSlice::setTexture(std::string textureName)
	{
		currentTextureName = textureName;
		setTexture(modules->texture->getTexture(textureName));
	}

	std::shared_ptr<SerializationClient> NineSlice::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__nine_slice__", hint);
		currentTextureName = client->serializeString("ctn", currentTextureName);
		currentAnimationSetName = client->serializeString("casn", currentAnimationSetName);

		if (!currentTextureName.empty() && texture == nullptr)
		{
			texture = modules->texture->getTexture(currentTextureName);
		}

		if (!currentAnimationSetName.empty() && animationSet == nullptr)
		{
			animationSet = modules->animation->getAnimationSet(currentAnimationSetName);
		}

		generateNineSliceAnimationSet();

		return Container::doSerialize(hint);
	}
}