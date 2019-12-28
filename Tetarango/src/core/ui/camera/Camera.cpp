#include "Camera.h"

namespace AW
{
	void Camera::notifyListener()
	{
		const auto cameraParent = std::dynamic_pointer_cast<Renderable>(getParent().lock());
		if (cameraParent != nullptr)
		{
			const auto zoom = getZoom();
			const auto anchorX = getScreenAnchorX();
			const auto anchorY = getScreenAnchorY();
			const auto screenWidth = modules->screen->getWidth();
			const auto screenHeight = modules->screen->getHeight();
			const auto pRect = cameraParent->getRect();
			const auto xOffset = screenWidth - anchorX;
			const auto yOffset = screenHeight - anchorY;
			const auto xZoomOffset = (anchorX - screenWidth / 2.0) * zoom - (anchorX - screenWidth / 2.0);
			const auto yZoomOffset = (anchorY - screenHeight / 2.0) * zoom - (anchorY - screenHeight / 2.0);

			cameraParent->setScale(zoom);
			cameraParent->setPosition(xOffset - xZoomOffset, yOffset - yZoomOffset);
		}

		const auto listenerPtr = std::dynamic_pointer_cast<ICameraListener>(listener.lock());
		if (listenerPtr != nullptr)
		{
			listenerPtr->onCameraUpdate(getId());
		}
	}

	void Camera::setScreenAnchorPoint(double xOffset, double yOffset)
	{
		xPosition = xOffset;
		yPosition = yOffset;
		notifyListener();
	}

	double Camera::getScreenAnchorX()
	{
		return xPosition;
	}

	double Camera::getScreenAnchorY()
	{
		return yPosition;
	}

	double Camera::getZoom()
	{
		return zoom;
	}

	void Camera::setZoom(double scale)
	{
		zoom = std::min(maxZoomIn, std::max(scale, maxZoomOut));
		notifyListener();
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

	void Camera::setDefaultZoomAndAnchorPoint(double zoom, double defaultXOffset, double defaultYOffset)
	{
		defaultZoom = zoom;
		defaultX = defaultXOffset;
		defaultY = defaultYOffset;
		reset();
	}

	void Camera::reset()
	{
		setScreenAnchorPoint(defaultX, defaultY);
		setZoom(defaultZoom);
	}

	std::shared_ptr<SerializationClient> Camera::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__camera__", hint);
		xPosition = client->serializeDouble("c-x", xPosition);
		yPosition = client->serializeDouble("c-y", yPosition);
		zoom = client->serializeDouble("c-z", zoom);
		maxZoomIn = client->serializeDouble("c-m-z-i", maxZoomIn);
		maxZoomOut = client->serializeDouble("c-m-z-o", maxZoomOut);
		defaultZoom = client->serializeDouble("c-d-z", defaultZoom);
		defaultX = client->serializeDouble("c-d-x", defaultX);
		defaultY = client->serializeDouble("c-d-y", defaultY);

		return GameObject::doSerialize(hint);
	}
}
