#include "GameCamera.h"

#include "gui/Guis.h"

namespace
{
	const auto targetRebindingIdName = "gc-rebind-t-id";
}

namespace AWGame
{
	GameCamera::GameCamera() : BaseGui(GuiCamera::CameraBasic)
	{
		GORegister(GameCamera);
	}

	void GameCamera::onInitialAttach()
	{
		Camera::onInitialAttach();

		modules->input->keyboard->registerKeys(std::vector<AWKey>{ AWKey::KP_8, AWKey::KP_2, AWKey::KP_5, AWKey::KP_6, AWKey::KP_4, AWKey::KP_PLUS, AWKey::KP_MINUS, AWKey::E, AWKey::C, AWKey::I, AWKey::J, AWKey::K, AWKey::L}, weak_from_this());
		modules->input->gamepad->registerAxis(0, AW::GamepadAxisMapping::RIGHT, weak_from_this());

		setTimeScope(AW::TimeScope::Camera);

		enableEnterFrame(5);
	}

	void GameCamera::onAttach()
	{
		const auto rebindingId = serializationClient->getInt(targetRebindingIdName);
		if (target.lock() == nullptr && rebindingId != 0)
		{
			target = getRootNode()->findChildWithBindingId<AW::Renderable>(rebindingId);
		}

		const auto goPtr = std::dynamic_pointer_cast<AW::GameObject>(target.lock());
		if (goPtr != nullptr)
		{
			serializationClient->setInt(targetRebindingIdName, goPtr->getBindingId());
		}
	}

	void GameCamera::onEnterFrame(const double& frameTime)
	{
		const auto targetPtr = target.lock();
		if (targetPtr != nullptr)
		{
			setScreenAnchorPoint(targetPtr->getScreenX(), targetPtr->getScreenY());
		}
		else
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

		if (modules->screen->getCurrentScreenConfig().visualizePhysic)
		{
			modules->physic->getDebugRenderer().matchWithCamera(shared_from_this());
		}
	}

	void GameCamera::onKey(AWKey code, bool pressed)
	{
		switch (code)
		{
		case AWKey::KP_8:
		case AWKey::I:
			upDown = pressed;
			break;

		case AWKey::KP_4:
		case AWKey::J:
			leftDown = pressed;
			break;

		case AWKey::KP_2:
		case AWKey::K:
			downDown = pressed;
			break;

		case AWKey::KP_6:
		case AWKey::L:
			rightDown = pressed;
			break;

		case AWKey::KP_PLUS:
		case AWKey::C:
			if (pressed)
			{
				setZoom(getZoom() * 2);
			}
			break;

		case AWKey::KP_MINUS:
		case AWKey::E:
			if (pressed)
			{
				setZoom(getZoom() / 2);
			}
			break;

		case AWKey::KP_5:
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


