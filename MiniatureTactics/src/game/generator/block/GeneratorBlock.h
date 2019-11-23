#pragma once

#include "util/Color.h"

namespace MTGame
{
	/*
	enum class HumanSkinColor
	{
		LightWhite,
		White,
		DarkWhite,
		Yellow,
		LightBlack,
		Black,
		DarkBlack,
		Brown,
		DarkBrown,
		Red,
		DarkRed
	};

	enum class HumanEyeColor
	{
		Brown,
		Blue,
		Hazel,
		Amber,
		Green,
		Grey,
		Red,
		Violet
	};

	enum class HumanHairColor
	{
		MidnightBlack,
		OffBlack,
		DarkBrown,
		MediumBrown,
		ChestnutBrown,
		LightChestnutBrown,
		DarkGoldenBrown,
		LightGoldenBrown,
		DarkHoneyBlond,
		BleachedBlonde,
		LightAshBlonde,
		LightAshBrown,
		LightBlonde,
		PaleGoldenBlonde,
		StrawberryBlonde,
		LightAuburn,
		DarkAuburn,
		DarkGrey,
		MediumGrey,
		LightGrey,
		WhiteBlonde,
		PlatinumBlonde,
		RussetRed,
		TerraCotta
	};
	*/

	class GeneratorBlock
	{
		static bool hasInit;
		void initGenerators();

	public:
		GeneratorBlock();
	};

}
