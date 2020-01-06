#pragma once

namespace AW
{
	// In-App understood mapping
	enum AxisMapping
	{
		LEFT_X,
		LEFT_Y,
		LEFT_TRIGGER,
		RIGHT_X,
		RIGHT_Y,
		RIGHT_TRIGGER
	};

	enum ButtonMapping
	{

		DOWN_ACTION,
		RIGHT_ACTION,
		LEFT_ACTION,
		UP_ACTION,
		LEFT_BUMPER,
		RIGHT_BUMPER,
		LEFT_SPECIAL,
		RIGHT_SPECIAL,
		LEFT_AXIS,
		RIGHT_AXIS,
		DPAD_CENTER,
		DPAD_UP,
		DPAD_RIGHT,
		DPAD_UP_RIGHT,
		DPAD_DOWN,
		DPAD_DOWN_RIGHT,
		DPAD_LEFT,
		DPAD_UP_LEFT,
		DPAD_DOWN_LEFT,
	};

	// General
	enum class GamepadAxisMapping
	{
		LEFT_X = AxisMapping::LEFT_X,
		LEFT_Y = AxisMapping::LEFT_Y,
		LEFT_TRIGGER = AxisMapping::LEFT_TRIGGER,
		RIGHT_X = AxisMapping::RIGHT_X,
		RIGHT_Y = AxisMapping::RIGHT_Y,
		RIGHT_TRIGGER = AxisMapping::RIGHT_TRIGGER,
		LEFT,
		RIGHT
	};

	enum class GamepadButtonMapping
	{
		DOWN_ACTION = ButtonMapping::DOWN_ACTION,
		RIGHT_ACTION = ButtonMapping::RIGHT_ACTION,
		LEFT_ACTION = ButtonMapping::LEFT_ACTION,
		UP_ACTION = ButtonMapping::UP_ACTION,
		LEFT_BUMPER = ButtonMapping::LEFT_BUMPER,
		RIGHT_BUMPER = ButtonMapping::RIGHT_BUMPER,
		LEFT_SPECIAL = ButtonMapping::LEFT_SPECIAL,
		RIGHT_SPECIAL = ButtonMapping::RIGHT_SPECIAL,
		LEFT_AXIS = ButtonMapping::LEFT_AXIS,
		RIGHT_AXIS = ButtonMapping::RIGHT_AXIS,

		DPAD_CENTER = ButtonMapping::DPAD_CENTER,
		DPAD_LEFT = ButtonMapping::DPAD_LEFT,
		DPAD_UP_LEFT = ButtonMapping::DPAD_UP_LEFT,
		DPAD_UP = ButtonMapping::DPAD_UP,
		DPAD_UP_RIGHT = ButtonMapping::DPAD_UP_RIGHT,
		DPAD_RIGHT = ButtonMapping::DPAD_RIGHT,
		DPAD_DOWN_RIGHT = ButtonMapping::DPAD_DOWN_RIGHT,
		DPAD_DOWN = ButtonMapping::DPAD_DOWN,
		DPAD_DOWN_LEFT = ButtonMapping::DPAD_DOWN_LEFT
	};

	// XBOX 360
	enum class Xbox360AxisMapping
	{
		LEFT_X = AxisMapping::LEFT_X,
		LEFT_Y = AxisMapping::LEFT_Y,
		LEFT_TRIGGER = AxisMapping::LEFT_TRIGGER,
		RIGHT_X = AxisMapping::RIGHT_X,
		RIGHT_Y = AxisMapping::RIGHT_Y,
		RIGHT_TRIGGER = AxisMapping::RIGHT_TRIGGER
	};

	enum class Xbox360ButtonMapping
	{
		A = ButtonMapping::DOWN_ACTION,
		B = ButtonMapping::RIGHT_ACTION,
		X = ButtonMapping::LEFT_ACTION,
		Y = ButtonMapping::UP_ACTION,
		LB = ButtonMapping::LEFT_BUMPER,
		RB = ButtonMapping::RIGHT_BUMPER,
		BACK = ButtonMapping::LEFT_SPECIAL,
		START = ButtonMapping::RIGHT_SPECIAL,
		LEFT_AXIS = ButtonMapping::LEFT_AXIS,
		RIGHT_AXIS = ButtonMapping::RIGHT_AXIS,
	};
}
