#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad/Gamepad.h"
#include "engine/module/IBaseModule.h"

namespace AWCore
{
	class Input : public IBaseModule
	{
	public:
		Input();
		std::shared_ptr<Keyboard> keyboard;
		std::shared_ptr<Mouse> mouse;
		std::shared_ptr<Gamepad> gamepad;

		void onCleanup();
	};
}
