#include "GameCamera.h"

#include "gui/Guis.h"

namespace AWGame
{
	GameCamera::GameCamera() : BaseGui(GuiCamera::CameraBasic)
	{
		registerGameObject<GameCamera>();
	}

	void GameCamera::onInitialAttach()
	{
		Camera::onInitialAttach();

		modules->input->keyboard->registerKeys(std::vector<SDL_Scancode>{ SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_6, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_PLUS, SDL_SCANCODE_KP_MINUS, SDL_SCANCODE_E, SDL_SCANCODE_C, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L}, weak_from_this());
		modules->input->gamepad->registerAxis(0, AW::GamepadAxisMapping::RIGHT, weak_from_this());

		setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		setTimeScope(AW::TimeScope::Camera);

		enableEnterFrame(1);
	}

	void GameCamera::onAttach()
	{
		GameObject::onAttach();

		if (isAttached())
		{
			enableCamera();
		}
	}

	void GameCamera::onDetach()
	{
		modules->screen->clearCamera();
	}

	void GameCamera::onEnterFrame(const double& frameTime)
	{
		if (xAxis || yAxis)
		{
			setX(getX() + xAxis * -1000.0 * (frameTime / 1000.0));
			setY(getY() + yAxis * -1000.0 * (frameTime / 1000.0));
		}

		double xDelta = 0.0, yDelta = 0.0;
		double speed = 1000.0;
		if (upDown)
		{
			yDelta = -speed / getZoom();
		}

		if (downDown)
		{
			yDelta = speed / getZoom();
		}

		if (leftDown)
		{
			xDelta = -speed / getZoom();
		}

		if (rightDown)
		{
			xDelta = speed / getZoom();
		}

		if (xDelta != 0.0)
		{
			setX(getX() + xDelta * (frameTime / 1000.0));
		}

		if (yDelta != 0.0)
		{
			setY(getY() + yDelta * (frameTime / 1000.0));
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


