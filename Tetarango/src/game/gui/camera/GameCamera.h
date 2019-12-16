#pragma once

#include "ui/Camera.h"
#include "gui/BaseGui.h"

namespace AWGame
{

	class GameCamera : public BaseGui, public AWCore::Camera
	{
		bool upDown = false, leftDown = false, downDown = false, rightDown = false, zoomInPressed = false, zoomOutPressed = false;
		double xAxis = 0.0, yAxis = 0.0;

	public:
		GameCamera();

		void onInitialAttach();
		void onAttach();
		void onDetach();
		void onEnterFrame(double frameTime);
		void onKey(SDL_Scancode code, bool pressed);
		void onGamepadAxisChanged(int gamepadIndex, AWCore::GamepadAxisMapping axis, double value);
	};
}
