#include "Camera.h"

namespace AWCore
{
	double Camera::getX()
	{
		return xPosition;
	}

	void Camera::setX(double x)
	{
		xPosition = x;
	}

	double Camera::getY()
	{
		return yPosition;
	}

	void Camera::setY(double y)
	{
		yPosition = y;
	}

	double Camera::getZoom()
	{
		return zoom;
	}

	void Camera::setZoom(double scale)
	{
		zoom = std::min(maxZoomIn, std::max(scale, maxZoomOut));
	}

	double Camera::getXOffset()
	{
		return centerX;
	}

	double Camera::getYOffset()
	{
		return centerY;
	}

	void Camera::setZoomAnchorPointOnScreen(double screenX, double screenY)
	{
		centerX = screenX;
		centerY = screenY;
	}

	void Camera::setZoomLimits(double maxZoomInFactor, double maxZoomOutFactor)
	{
		if (maxZoomInFactor > 0)
		{
			maxZoomIn = std::max(maxZoomInFactor, maxZoomOutFactor);
			maxZoomOut = std::min(maxZoomInFactor, maxZoomOutFactor);
			maxZoomOut = std::max(0.0, maxZoomOut);
		}

		setZoom(zoom);
	}

	void Camera::setDefaults(double zoom, double xOffset, double yOffset)
	{
		defaultZoom = zoom;
		defaultX = xOffset;
		defaultY = yOffset;
		reset();
	}

	void Camera::setDefaultsAndReset(double zoom, double xOffset, double yOffset)
	{
		setDefaults(zoom, xOffset, yOffset);
		reset();
	}

	void Camera::reset()
	{
		setZoom(defaultZoom);
		setX(defaultX);
		setY(defaultY);
	}

	void Camera::enableCamera()
	{
		const auto cameraRef = std::dynamic_pointer_cast<Camera>(shared_from_this());
		modules->screen->setCamera(cameraRef);
	}

	std::shared_ptr<SerializationClient> Camera::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__camera__", hint);
		xPosition = client->serializeDouble("x", xPosition);
		yPosition = client->serializeDouble("y", yPosition);
		zoom = client->serializeDouble("z", zoom);
		centerX = client->serializeDouble("cX", centerX);
		centerY = client->serializeDouble("cY", centerY);
		maxZoomIn = client->serializeDouble("maxZI", maxZoomIn);
		maxZoomOut = client->serializeDouble("maxZO", maxZoomOut);
		defaultZoom = client->serializeDouble("dZ", defaultZoom);
		defaultX = client->serializeDouble("dX", defaultX);
		defaultY = client->serializeDouble("dY", defaultY);

		return ApplicationObject::doSerialize(hint);
	}
}
