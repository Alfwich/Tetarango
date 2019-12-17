#pragma once

#include <algorithm>
#include <SDL.h>
#include "engine/ApplicationObject.h"

namespace AW
{

	class Camera : public ApplicationObject
	{
		double xPosition = 0.0, yPosition = 0.0, zoom = 1.0, centerX = 0.0, centerY = 0.0, maxZoomIn = 5.0, maxZoomOut = 0.2;
		double defaultZoom = 1.0, defaultX = 0.0, defaultY = 0.0;

	public:
		double getX();
		void setX(double x);

		double getY();
		void setY(double x);

		double getZoom();
		void setZoom(double scale);

		double getXOffset();
		double getYOffset();

		void setZoomAnchorPointOnScreen(double screenX, double screenY);
		void setZoomLimits(double maxZoomInFactor, double maxZoomOutFactor);

		void setDefaults(double zoom, double xOffset, double yOffset);
		void setDefaultsAndReset(double zoom, double xOffset, double yOffset);
		void reset();

		void enableCamera();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
