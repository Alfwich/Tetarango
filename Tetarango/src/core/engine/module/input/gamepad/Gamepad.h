#pragma once

#include "GamepadMappings.h"
#include "engine/module/input/IInputListener.h"

namespace AW
{

	class GamepadIDBundle
	{
	public:
		SDL_Joystick* joystick;
		SDL_Haptic* haptic;
	};

	class Gamepad
	{
		int stickAxisDeadzone;
		bool hapticsEnabled;
		std::unordered_map<std::string, int> GUIDToAssociatedIndex;
		std::unordered_map<int, int> instanceIdToAssociatedIdOverride;
		std::unordered_map<int, std::unique_ptr<GamepadIDBundle>> deviceIdToGamepadBundle;
		std::unordered_map<int, std::map<GamepadAxisMapping, std::list<std::weak_ptr<IInputListener>>>> axisListeners;
		std::unordered_map<int, std::map<GamepadButtonMapping, std::list<std::weak_ptr<IInputListener>>>> buttonListeners;

		void handleAxisEvent(SDL_Event* event);
		double processAxisValue(GamepadAxisMapping axis, int axisValue);
		double processAxisStickValue(int axisValue);
		double processAxisTriggerValue(int axisValue);
		void handleButtonEvent(SDL_Event* event);
		void handleHatEvent(SDL_Event* event);
		void fireButtonEvent(int gamepadIndex, GamepadButtonMapping button, bool pressed);
		int mapDeviceIdToBoundId(int deviceId);
		std::string getGamepadGUID(SDL_Joystick* gamepad);
		bool openGamepad(int index = 0);
		void closeGamepad(int index);

	public:
		Gamepad();
		~Gamepad();

		int currentOpenGamepads();

		void setAxisDeadzone(int deadzone);
		int getAxisDeadzone();

		void registerAxis(int gamepadIndex, GamepadAxisMapping axis, std::weak_ptr<IInputListener> listener);
		void registerButton(int gamepadIndex, GamepadButtonMapping button, std::weak_ptr<IInputListener> listener);
		void unregisterAxis(int gamepadIndex, GamepadAxisMapping axis, std::weak_ptr<IInputListener> listener);
		void unregisterButton(int gamepadIndex, GamepadButtonMapping button, std::weak_ptr<IInputListener> listener);
		void unregisterObject(std::weak_ptr<IInputListener> listener);

		void enableHaptics();
		void disableHaptics();
		void fireHapticForGamepad(int gamepadIndex, double intensity, int duration);

		void updateGamepadState(SDL_Event* event);
		void updateGamepadConnectedState(SDL_Event* event);
		void purgeWeakRefs();

		void closeAllOpenGamepads();
	};

}
