#pragma once

#include <SDL.h>
#include "gamepad/GamepadMappings.h"

namespace MT
{

	enum class MouseButton {
		Left,
		Right,
		Middle
	};

	class IInputListener {
	public:
		bool inputEnabled = false;
		virtual int inputListenerObjectId() { return 0; };
		virtual void onKey(SDL_Scancode code, bool pressed) { /* NO-OP */ };
		virtual void onKeyPressed(SDL_Scancode code) { /* NO-OP */ };
		virtual void onKeyReleased(SDL_Scancode code) { /* NO-OP */ };

		virtual void onMouseMove(int x, int y) { /* NO-OP */ };
		virtual void onMouseButton(MouseButton, bool pressed) { /* NO-OP */ };
		virtual void onMouseButtonLeftDown() { /* NO-OP */ };
		virtual void onMouseButtonLeftUp() { /* NO-OP */ };
		virtual void onMouseButtonRightDown() { /* NO-OP */ };
		virtual void onMouseButtonRightUp() { /* NO-OP */ };
		virtual void onMouseButtonMiddleDown() { /* NO-OP */ };
		virtual void onMouseButtonMiddleUp() { /* NO-OP */ };

		virtual void onGamepadReassigned(int newIndex) { /* NO-OP */ };
		virtual void onGamepadAxisChanged(int gamepadIndex, GamepadAxisMapping axis, double value) { /* NO-OP */ };
		virtual void onGamepadButton(int gamepadIndex, GamepadButtonMapping button, bool pressed) { /* NO-OP */ };
		virtual void onGamepadButtonDown(int gamepadIndex, GamepadButtonMapping button) { /* NO-OP */ };
		virtual void onGamepadButtonUp(int gamepadIndex, GamepadButtonMapping button) { /* NO-OP */ };

		void key(SDL_Scancode code, bool pressed)
		{
			if (!this->inputEnabled) return;

			this->onKey(code, pressed);
			if (pressed) this->onKeyPressed(code); else this->onKeyReleased(code);
		}

		void mouseButton(MouseButton button, bool pressed)
		{
			if (!this->inputEnabled) return;

			this->onMouseButton(button, pressed);
			switch (button)
			{
			case MouseButton::Left:
				if (pressed) this->onMouseButtonLeftDown(); else this->onMouseButtonLeftUp();
				break;

			case MouseButton::Right:
				if (pressed) this->onMouseButtonRightDown(); else this->onMouseButtonRightUp();
				break;

			case MouseButton::Middle:
				if (pressed) this->onMouseButtonMiddleDown(); else this->onMouseButtonMiddleUp();
				break;
			}
		}

		virtual void mouseMove(int x, int y) {
			if (this->inputEnabled) this->onMouseMove(x, y);
		};

		void gamepadAxis(int gamepadIndex, GamepadAxisMapping axis, double value)
		{
			if (!this->inputEnabled) return;

			this->onGamepadAxisChanged(gamepadIndex, axis, value);
		}

		void gamepadButton(int gamepadIndex, GamepadButtonMapping button, bool pressed)
		{
			if (!this->inputEnabled) return;

			this->onGamepadButton(gamepadIndex, button, pressed);
			if (pressed) this->onGamepadButtonDown(gamepadIndex, button); else this->onGamepadButtonUp(gamepadIndex, button);
		}
	};
}
