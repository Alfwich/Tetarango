#include "Gamepad.h"
namespace AWCore
{

	Gamepad::Gamepad()
	{
		stickAxisDeadzone = 8000;
		hapticsEnabled = true;
	}

	Gamepad::~Gamepad()
	{
		for (auto it = deviceIdToGamepadBundle.begin(); it != deviceIdToGamepadBundle.end();)
		{
			if (SDL_JoystickGetAttached(it->second->joystick))
			{
				closeGamepad((it++)->first);
			}
			else {
				++it;
			}
		}
	}

	void Gamepad::updateGamepadState(SDL_Event* event)
	{
		if (currentOpenGamepads() < 0)
		{
			return;
		}

		switch (event->type)
		{
		case SDL_JOYAXISMOTION:
			handleAxisEvent(event);
			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			handleButtonEvent(event);
			break;
		case SDL_JOYHATMOTION:
			handleHatEvent(event);
			break;
		}
	}

	void Gamepad::updateGamepadConnectedState(SDL_Event* event)
	{
		switch (event->type)
		{
		case SDL_JOYDEVICEADDED:
			openGamepad(event->jdevice.which);
			break;

		case SDL_JOYDEVICEREMOVED:
			closeGamepad(event->jdevice.which);
			break;
		}
	}

	void Gamepad::purgeWeakRefs()
	{
		for (auto& gamepadGroup : buttonListeners)
		{
			for (auto& buttonListener : gamepadGroup.second)
			{
				for (auto it = buttonListener.second.begin(); it != buttonListener.second.end();)
				{
					const auto ptr = (*it).lock();
					if (ptr == nullptr)
					{
						it = buttonListener.second.erase(it);
					}
					else 
					{
						++it;
					}
				}
			}
		}

		for (auto& gamepadGroup : axisListeners)
		{
			for (auto& axisListener : gamepadGroup.second)
			{
				for (auto it = axisListener.second.begin(); it != axisListener.second.end();)
				{
					const auto ptr = (*it).lock();
					if (ptr == nullptr)
					{
						it = axisListener.second.erase(it);
					}
					else 
					{
						++it;
					}
				}
			}
		}
	}

	void Gamepad::handleAxisEvent(SDL_Event* event)
	{
		int gamepadIndex = mapDeviceIdToBoundId(event->jaxis.which);
		GamepadAxisMapping mappedAxis = (GamepadAxisMapping)event->jaxis.axis;

		for (const auto listener : axisListeners[gamepadIndex][mappedAxis])
		{
			const auto ptr = listener.lock();
			if (ptr != nullptr)
			{
				ptr->gamepadAxis(event->jaxis.which, mappedAxis, processAxisValue(mappedAxis, event->jaxis.value));
			}
		}
	}

	double Gamepad::processAxisValue(GamepadAxisMapping axis, int axisValue)
	{
		switch (axis)
		{
		case GamepadAxisMapping::LEFT_X:
		case GamepadAxisMapping::LEFT_Y:
		case GamepadAxisMapping::RIGHT_X:
		case GamepadAxisMapping::RIGHT_Y:
			return processAxisStickValue(axisValue);

		case GamepadAxisMapping::LEFT_TRIGGER:
		case GamepadAxisMapping::RIGHT_TRIGGER:
			return processAxisTriggerValue(axisValue);
		}

		return axisValue < 0
			? axisValue / 32768.0
			: axisValue / 32767.0;
	}

	double Gamepad::processAxisStickValue(int axisValue)
	{
		if (std::abs(axisValue) < stickAxisDeadzone)
		{
			return 0.0;
		}

		return (axisValue < 0)
			? (axisValue + stickAxisDeadzone) / (32768.0 - stickAxisDeadzone)
			: (axisValue - stickAxisDeadzone) / (32767.0 - stickAxisDeadzone);
	}

	double Gamepad::processAxisTriggerValue(int axisValue)
	{
		double offset = axisValue < 0
			? axisValue / 32768.0
			: axisValue / 32767.0;

		return (offset + 1.0) / 2.0;
	}

	void Gamepad::handleButtonEvent(SDL_Event* event)
	{
		int gamepadIndex = mapDeviceIdToBoundId(event->jaxis.which);
		GamepadButtonMapping mappedButton = (GamepadButtonMapping)event->jbutton.button;
		fireButtonEvent(gamepadIndex, mappedButton, event->jbutton.state == SDL_PRESSED);
	}

	void Gamepad::fireButtonEvent(int gamepadIndex, GamepadButtonMapping button, bool pressed)
	{
		for (const auto listener : buttonListeners[gamepadIndex][button])
		{
			const auto ptr = listener.lock();
			if (ptr != nullptr)
			{
				ptr->gamepadButton(gamepadIndex, button, pressed);
			}
		}
	}

	int Gamepad::mapDeviceIdToBoundId(int deviceId)
	{
		if (instanceIdToAssociatedIdOverride.count(deviceId) == 1)
		{
			return instanceIdToAssociatedIdOverride[deviceId];
		}

		return deviceId;
	}

	std::string Gamepad::getGamepadGUID(SDL_Joystick* gamepad)
	{
		char b[128];
		SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(gamepad), b, 128);
		return std::string(b);
	}

	void Gamepad::handleHatEvent(SDL_Event* event)
	{
		int gamepadIndex = mapDeviceIdToBoundId(event->jaxis.which);
		switch (event->jhat.value)
		{
		case SDL_HAT_CENTERED:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_CENTER, true);
			break;
		case SDL_HAT_UP:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_UP, true);
			break;
		case SDL_HAT_RIGHT:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_RIGHT, true);
			break;
		case SDL_HAT_RIGHTUP:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_UP_RIGHT, true);
			break;
		case SDL_HAT_DOWN:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_DOWN, true);
			break;
		case SDL_HAT_RIGHTDOWN:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_DOWN_RIGHT, true);
			break;
		case SDL_HAT_LEFT:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_LEFT, true);
			break;
		case SDL_HAT_LEFTUP:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_UP_LEFT, true);
			break;
		case SDL_HAT_LEFTDOWN:
			fireButtonEvent(gamepadIndex, GamepadButtonMapping::DPAD_DOWN_LEFT, true);
			break;
		}
	}

	bool Gamepad::openGamepad(int index)
	{
		if (SDL_NumJoysticks() < 0 || !(SDL_WasInit(SDL_INIT_JOYSTICK) & SDL_INIT_JOYSTICK)) {
			Logger::instance()->logCritical("Gamepad::Attempted to open gamepad when SDL_Joystick was not init");
			return false;
		}

		SDL_Joystick* gamepad = SDL_JoystickOpen(index);

		if (gamepad)
		{
			std::unique_ptr<GamepadIDBundle> bundle = std::make_unique<GamepadIDBundle>();
			bundle->joystick = gamepad;
			auto hapticController = SDL_HapticOpenFromJoystick(gamepad);
			if (hapticController && !SDL_HapticRumbleInit(hapticController))
			{
				bundle->haptic = hapticController;
				SDL_HapticRumblePlay(hapticController, 0.75, 150);
			}

			int instanceId = SDL_JoystickInstanceID(gamepad);
			std::string GUID = getGamepadGUID(gamepad);

			if (GUIDToAssociatedIndex.count(GUID) == 1)
			{
				instanceIdToAssociatedIdOverride[instanceId] = GUIDToAssociatedIndex[GUID];
			}

			if (GUIDToAssociatedIndex.count(GUID) == 0)
			{
				GUIDToAssociatedIndex[GUID] = (int)GUIDToAssociatedIndex.size();
				instanceIdToAssociatedIdOverride[instanceId] = GUIDToAssociatedIndex[GUID];
			}

			int gamepadIndex = mapDeviceIdToBoundId(index);
			deviceIdToGamepadBundle[gamepadIndex] = std::move(bundle);
			Logger::instance()->log("Gamepad::Opened gamepad GUID=" + GUID + ", AssociatedId=" + std::to_string(GUIDToAssociatedIndex[GUID]) + ", InstanceId=" + std::to_string(instanceId) + ", ConnectId=" + std::to_string(index));
		}
		else
		{
			Logger::instance()->logCritical("Gamepad::Failed to open gamepad DeviceId: " + std::to_string(index));
		}

		return (bool)gamepad;
	}

	void Gamepad::closeGamepad(int index)
	{
		if (deviceIdToGamepadBundle.size() <= 0 || !(SDL_WasInit(SDL_INIT_JOYSTICK) & SDL_INIT_JOYSTICK)) {
			return;
		}

		int gamepadIndex = mapDeviceIdToBoundId(index);
		SDL_Joystick* gamepad = deviceIdToGamepadBundle[gamepadIndex]->joystick;
		SDL_Haptic* haptic = deviceIdToGamepadBundle[gamepadIndex]->haptic;
		std::string GUID = getGamepadGUID(gamepad);
		int instanceId = SDL_JoystickInstanceID(gamepad);
		if (SDL_JoystickGetAttached(gamepad) == SDL_TRUE)
		{
			SDL_JoystickClose(gamepad);
		}

		if (haptic)
		{
			SDL_HapticClose(haptic);
		}

		Logger::instance()->log("Gamepad::Closed gamepad GUID=" + GUID + ", AssociatedId=" + std::to_string(GUIDToAssociatedIndex[GUID]) + ", InstanceId=" + std::to_string(instanceId));

		deviceIdToGamepadBundle.erase(gamepadIndex);
	}

	int Gamepad::currentOpenGamepads()
	{
		return (int)deviceIdToGamepadBundle.size();
	}

	void Gamepad::setAxisDeadzone(int deadzone)
	{
		stickAxisDeadzone = std::abs(deadzone);
	}

	int Gamepad::getAxisDeadzone()
	{
		return stickAxisDeadzone;
	}

	void Gamepad::registerAxis(int gamepadIndex, GamepadAxisMapping axis, std::weak_ptr<IInputListener> listener)
	{
		if (axis == GamepadAxisMapping::LEFT) {
			registerAxis(gamepadIndex, GamepadAxisMapping::LEFT_X, listener);
			registerAxis(gamepadIndex, GamepadAxisMapping::LEFT_Y, listener);
			return;
		}
		else if (axis == GamepadAxisMapping::RIGHT)
		{
			registerAxis(gamepadIndex, GamepadAxisMapping::RIGHT_X, listener);
			registerAxis(gamepadIndex, GamepadAxisMapping::RIGHT_Y, listener);
			return;
		}

		axisListeners[gamepadIndex][axis].push_back(listener);
	}

	void Gamepad::registerButton(int gamepadIndex, GamepadButtonMapping button, std::weak_ptr<IInputListener> listener)
	{
		buttonListeners[gamepadIndex][button].push_back(listener);
	}

	void Gamepad::unregisterAxis(int gamepadIndex, GamepadAxisMapping axis, std::weak_ptr<IInputListener> listener)
	{
		if (axis == GamepadAxisMapping::LEFT) {
			unregisterAxis(gamepadIndex, GamepadAxisMapping::LEFT_X, listener);
			unregisterAxis(gamepadIndex, GamepadAxisMapping::LEFT_Y, listener);
			return;
		}
		else if (axis == GamepadAxisMapping::RIGHT)
		{
			unregisterAxis(gamepadIndex, GamepadAxisMapping::RIGHT_X, listener);
			unregisterAxis(gamepadIndex, GamepadAxisMapping::RIGHT_Y, listener);
			return;
		}

		const auto objectToRemovePtr = listener.lock();
		for (auto it = axisListeners[gamepadIndex][axis].begin(); it != axisListeners[gamepadIndex][axis].end();)
		{
			if (const auto ptr = (*it).lock())
			{
				if (ptr == objectToRemovePtr)
				{
					axisListeners[gamepadIndex][axis].erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
			else
			{
				it = axisListeners[gamepadIndex][axis].erase(it);
			}
		}
	}

	void Gamepad::unregisterButton(int gamepadIndex, GamepadButtonMapping button, std::weak_ptr<IInputListener> listener)
	{
		const auto objectToRemovePtr = listener.lock();
		for (auto it = buttonListeners[gamepadIndex][button].begin(); it != buttonListeners[gamepadIndex][button].end();)
		{
			if (const auto ptr = (*it).lock())
			{
				if (ptr == objectToRemovePtr)
				{
					buttonListeners[gamepadIndex][button].erase(it);
					return;
				}
				else
				{
					++it;
				}
			}
			else
			{
				it = buttonListeners[gamepadIndex][button].erase(it);
			}
		}
	}

	void Gamepad::unregisterObject(std::weak_ptr<IInputListener> listener)
	{
		for (const auto gamepadAxisListeners : axisListeners)
		{
			for (const auto axisListener : gamepadAxisListeners.second)
			{
				unregisterAxis(gamepadAxisListeners.first, axisListener.first, listener);
			}
		}

		for (const auto gamepadButtonListeners : buttonListeners)
		{
			for (const auto buttonListener : gamepadButtonListeners.second)
			{
				unregisterButton(gamepadButtonListeners.first, buttonListener.first, listener);
			}

		}
	}

	void Gamepad::enableHaptics()
	{
		hapticsEnabled = true;
	}

	void Gamepad::disableHaptics()
	{
		hapticsEnabled = false;
	}

	void Gamepad::fireHapticForGamepad(int index, double intensity, int duration)
	{
		SDL_Haptic* haptic = deviceIdToGamepadBundle[index]->haptic;
		if (hapticsEnabled && haptic)
		{
			SDL_HapticRumblePlay(haptic, (float)intensity, duration);
		}
	}

	void Gamepad::closeAllOpenGamepads()
	{
		for (unsigned int i = 0; i < deviceIdToGamepadBundle.size(); ++i)
		{
			closeGamepad(i);
		}
	}

}
