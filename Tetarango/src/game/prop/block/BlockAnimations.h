#pragma once

#include "engine/module/animation/AnimationSet.h"
#include "Blocks.h"
#include "util/Rect.h"

namespace AWGame
{

	class BlockAnimations : public MT::AnimationSet
	{
		const Blocks& config;

	public:
		BlockAnimations(const Blocks& cfg) : config(cfg) {};

		void onLoadAnimations()
		{
			{
				auto anim = std::shared_ptr<MT::Animation>();
				auto index = 0;

				// Head Apparel
				for (const auto blockEntry : config.blocks)
				{
					const std::string blockName = blockEntry.second;
					const MT::RectI offset = {
						0, 64 * index, 64, 64
					};

					// Default animation
					anim = startNewAnimation(blockName);
					anim->addGeneralFrames(offset.x + offset.w * 0, offset.y, offset.w, offset.h, 1);

					++index;
				}
			}

		}
	};

}
