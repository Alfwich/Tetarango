#pragma once

#include <algorithm>
#include <SDL.h>
#include "engine/GameObject.h"
#include "ICameraListener.h"

namespace AW
{
	class Camera : public GameObject
	{
		double xPosition = 0.0, yPosition = 0.0, zoom = 1.0, maxZoomIn = 64.0, maxZoomOut = 0.0;
		double defaultZoom = 1.0, defaultX = 0.0, defaultY = 0.0;

		void notifyListener();

	public:

		double getZoom();
		void setZoom(double scale);
		void setZoomLimits(double maxZoomInFactor, double maxZoomOutFactor);
		void setScreenAnchorPoint(double xOffset, double yOffset);
		double getScreenAnchorX();
		double getScreenAnchorY();
		void setDefaultZoomAndAnchorPoint(double zoom, double xOffset, double yOffset);

		std::weak_ptr<GameObject> listener;

		void reset();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
