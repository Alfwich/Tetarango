#pragma once

#include <string>
#include <map>
#include "util/Rect.h"

namespace AWGame
{

	enum class BlockTypes
	{
		Basic
	};

	class Blocks 
	{
	public:
		std::string apparelAnimationKey = "prop-block";

		// Head
		std::string blockTexturePath = "res/game/img/prop/block/blocks.png";
		AWCore::RectI blockTextureSize = { 0, 0, 512, 512 };
		const std::map<BlockTypes, std::string> blocks{
			{ BlockTypes::Basic, "block-basic" }
		};
	};
}

