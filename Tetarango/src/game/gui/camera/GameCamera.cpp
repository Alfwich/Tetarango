#include "GameCamera.h"

#include "gui/Guis.h"

namespace AWGame
{
	GameCamera::GameCamera() : BaseGui(GuiCamera::CameraBasic)
	{
		registerGameObject<GameCamera>(__FUNCTION__);
	}

	void GameCamera::onInitialAttach()
	{
		Camera::onInitialAttach();

		modules->input->keyboard->registerKeys(std::vector<SDL_Scancode>{ SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_6, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_PLUS, SDL_SCANCODE_KP_MINUS, SDL_SCANCODE_E, SDL_SCANCODE_C, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L}, weak_from_this());
		modules->input->gamepad->registerAxis(0, AW::GamepadAxisMapping::RIGHT, weak_from_this());

		setTimeScope(AW::TimeScope::Camera);

		enableEnterFrame(1);
	}

	void GameCamera::onEnterFrame(const double& frameTime)
	{
		const auto deltaTime = frameTime / 1000.0;
		if (xAxis || yAxis)
		{
			setScreenAnchorPoint(getScreenAnchorX() + xAxis * deltaTime, getScreenAnchorY() + yAxis * deltaTime);
		}

		double xDelta = 0.0, yDelta = 0.0;
		double speed = -1000.0 * deltaTime;
		if (upDown)
		{
			yDelta = speed;
		}

		if (downDown)
		{
			yDelta = -speed;
		}

		if (leftDown)
		{
			xDelta = speed;
		}

		if (rightDown)
		{
			xDelta = -speed;
		}

		if (xDelta != 0.0 || yDelta != 0.0)
		{
			setScreenAnchorPoint(getScreenAnchorX() + xDelta, getScreenAnchorY() + yDelta);
		}
	}

	void GameCamera::onKey(SDL_Scancode code, bool pressed)
	{
		switch (code)
		{
		case SDL_SCANCODE_KP_8:
		case SDL_SCANCODE_I:
			upDown = pressed;
			break;

		case SDL_SCANCODE_KP_4:
		case SDL_SCANCODE_J:
			leftDown = pressed;
			break;

		case SDL_SCANCODE_KP_2:
		case SDL_SCANCODE_K:
			downDown = pressed;
			break;

		case SDL_SCANCODE_KP_6:
		case SDL_SCANCODE_L:
			rightDown = pressed;
			break;

		case SDL_SCANCODE_KP_PLUS:
		case SDL_SCANCODE_C:
			if (pressed)
			{
				setZoom(getZoom() * 2);
			}
			break;

		case SDL_SCANCODE_KP_MINUS:
		case SDL_SCANCODE_E:
			if (pressed)
			{
				setZoom(getZoom() / 2);
			}
			break;

		case SDL_SCANCODE_KP_5:
			reset();
			break;
		}
	}

	void GameCamera::onGamepadAxisChanged(int gamepadIndex, AW::GamepadAxisMapping axis, double value)
	{
		switch (axis)
		{
		case AW::GamepadAxisMapping::RIGHT_X:
			xAxis = -value;
			break;

		case AW::GamepadAxisMapping::RIGHT_Y:
			yAxis = -value;
			break;
		}
	}

}


