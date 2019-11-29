#pragma once

#include "Element.h"

namespace MT
{

	class NineSlice : public Element
	{
		std::shared_ptr<MT::AnimationSet> nineSliceAnimationSet;

		void generateAnimationSet();

	public:
		NineSlice();

		void setCornerSize(unsigned int cornerSize);

		void onCreateChildren();
		void onChildrenHydrated();

		void setTexture(std::shared_ptr<Texture> texture);
		void setTexture(std::string textureName);
	};
}
