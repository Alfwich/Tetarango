#pragma once

#include "ui/camera/Camera.h"
#include "gui/BaseGui.h"

namespace AWGame
{

	class GameCamera : public BaseGui, public AW::Camera
	{
		bool upDown = false, leftDown = false, downDown = false, rightDown = false, zoomInPressed = false, zoomOutPressed = false;
		double xAxis = 0.0, yAxis = 0.0;

	public:
		GameCamera();

		void onInitialAttach();
		void onAttach();
		void onEnterFrame(const double& frameTime);
		void onKey(AWKey code, bool pressed);
		void onGamepadAxisChanged(int gamepadIndex, AW::GamepadAxisMapping axis, double value);

		std::weak_ptr<AW::Renderable> target;
	};
}
