#pragma once

namespace AWCore
{
	enum class TimeScope
	{
		None,
		ApplicationFrameTimer,
		ApplicationFrameLimitTimer,
		Global,
		Game,
		Menu,
		Camera
	};
}