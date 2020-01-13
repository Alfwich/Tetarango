#pragma once

#include "ui/renderable/primitive/Primitive.h"
#include "engine/module/time/timer.h"

namespace AW
{

	class Trace : public Primitive
	{
		double numSegments = 5, length = 0.0;
		bool firstRender = true;
		Color endColor;
		Rect first;

	public:
		Trace();

		void onInitialAttach();
		void setNumberOfSegments(double segments);
		void setEndColor(Color c);
		void setLength(double length);

		void preRender(Rect* renderRect, RenderPackage* renderPackage);

		Color getSegmentColor(unsigned int pos);
	};

}
