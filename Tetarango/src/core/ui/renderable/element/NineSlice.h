#pragma once

#include "ui/renderable/element/Animated.h"
#include "ui/renderable/container/Container.h"

namespace MT
{

	class NineSlice : public Container
	{
		std::shared_ptr<Texture> texture;
		std::shared_ptr<AnimationSet> animationSet;
		std::shared_ptr<AnimationSet> nineSliceAnimationSet;

		std::string currentTextureName, currentAnimationSetName;

		void generateNineSliceAnimationSet();

		void pushAnimation(std::string prefix, int cornerSize, int x, int y, int width, int height);

		std::shared_ptr<Animated> topLeft;
		std::shared_ptr<Animated> top;
		std::shared_ptr<Animated> topRight;

		std::shared_ptr<Animated> left;
		std::shared_ptr<Animated> center;
		std::shared_ptr<Animated> right;

		std::shared_ptr<Animated> bottomLeft;
		std::shared_ptr<Animated> bottom;
		std::shared_ptr<Animated> bottomRight;

	public:
		NineSlice();

		void setCornerSize(unsigned int cornerSize);

		void onCreateChildren();
		void onChildrenHydrated();
		void onLayoutChildren();

		void play(std::string animationName = "");

		void setAnimationSet(std::string animationSetName);
		void setAnimationSet(std::shared_ptr<AnimationSet> animationSet);

		void setTexture(std::shared_ptr<Texture> texture);
		void setTexture(std::string textureName);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
