#include "Input.h"

namespace AWCore
{
	Input::Input()
	{
		keyboard = std::make_unique<Keyboard>();
		mouse = std::make_unique<Mouse>();
		gamepad = std::make_unique<Gamepad>();
	}

	void Input::onCleanup()
	{
		gamepad->closeAllOpenGamepads();
	}
}
