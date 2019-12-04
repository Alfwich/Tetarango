#pragma once

#include "Animated.h"

namespace MT
{

	class NineSlice : public Animated
	{
		std::shared_ptr<AnimationSet> nineSliceAnimationSet;

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

		void setTexture(std::shared_ptr<Texture> texture);
		void setTexture(std::string textureName);
		void setColor(int r, int g, int b, int a = 0xff);
	};
}
