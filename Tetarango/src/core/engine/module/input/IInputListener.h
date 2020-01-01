#pragma once

#include <SDL.h>
#include "gamepad/GamepadMappings.h"
#include "util/Key.h"

namespace AW
{
	enum class InputMode {
		Unspecified,
		Enabled,
		ParentEnabled,
		Disabled
	};


	class IInputListener {
		InputMode inputMode = InputMode::Unspecified;

	public:
		virtual void enableInput() { inputMode = InputMode::Enabled; }
		virtual void disableInput() { inputMode = InputMode::Disabled; }
		virtual void setInputEnabled(bool enabled) { enabled ? enableInput() : disableInput(); }
		virtual InputMode getInputMode() { return inputMode; }
		virtual void setInputMode(InputMode mode) { inputMode = mode; }
		virtual bool getInputEnabled() { return inputMode == InputMode::Enabled || inputMode == InputMode::ParentEnabled; }

		virtual int inputListenerObjectId() { return 0; };
		virtual void onKey(AWKey code, bool pressed) { /* NO-OP */ };
		virtual void onKeyPressed(AWKey code) { /* NO-OP */ };
		virtual void onKeyReleased(AWKey code) { /* NO-OP */ };

		virtual void onMouseMove(int x, int y) { /* NO-OP */ };
		virtual void onMouseWheel(int x, int y) { /* NO-OP */ };
		virtual void onMouseButton(AWMouseButton, bool pressed) { /* NO-OP */ };
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

		void key(AWKey code, bool pressed)
		{
			if (!getInputEnabled()) return;

			this->onKey(code, pressed);
			if (pressed) this->onKeyPressed(code); else this->onKeyReleased(code);
		}

		void mouseButton(AWMouseButton button, bool pressed)
		{
			if (!getInputEnabled()) return;

			this->onMouseButton(button, pressed);
			switch (button)
			{
			case AWMouseButton::Left:
				if (pressed) this->onMouseButtonLeftDown(); else this->onMouseButtonLeftUp();
				break;

			case AWMouseButton::Right:
				if (pressed) this->onMouseButtonRightDown(); else this->onMouseButtonRightUp();
				break;

			case AWMouseButton::Middle:
				if (pressed) this->onMouseButtonMiddleDown(); else this->onMouseButtonMiddleUp();
				break;
			}
		}

		virtual void mouseMove(int x, int y) {
			if (!getInputEnabled()) return;

			this->onMouseMove(x, y);
		};

		virtual void mouseWheel(int x, int y) {
			if (!getInputEnabled()) return;
			
			this->onMouseWheel(x, y);
		};

		void gamepadAxis(int gamepadIndex, GamepadAxisMapping axis, double value)
		{
			if (!getInputEnabled()) return;

			this->onGamepadAxisChanged(gamepadIndex, axis, value);
		}

		void gamepadButton(int gamepadIndex, GamepadButtonMapping button, bool pressed)
		{
			if (!getInputEnabled()) return;

			this->onGamepadButton(gamepadIndex, button, pressed);
			if (pressed) this->onGamepadButtonDown(gamepadIndex, button); else this->onGamepadButtonUp(gamepadIndex, button);
		}
	};
}
