#include "Trace.h"

#include "util/NumberHelper.h"
#include "ui/renderable/element/Rectangle.h"

namespace MT
{
	Trace::Trace()
	{
		enableSerialization<Trace>();
	}

	void Trace::onInitialAttach()
	{
		for (int i = 0; i < numSegments; ++i)
		{
			add(std::make_shared<MT::Rectangle>());
		}

		auto baseColor = getColor();
		int segmentPos = 0;
		for (const auto rect : getChildrenOfType<MT::Rectangle>())
		{
			rect->setColor(baseColor.lerp(endColor, segmentPos++ / numSegments));
		}
	}

	void Trace::setNumberOfSegments(double segments)
	{
		numSegments = segments;
	}

	void Trace::setEndColor(Color c)
	{
		endColor = c;
	}

	void Trace::setLength(double length)
	{
		this->length = std::abs(length);
	}

	void Trace::preRender(Rect* renderRect, RenderPackage* renderPackage)
	{
		if (firstRender)
		{
			first = *getWorldRect();
			firstRender = false;
			return;
		}

		auto tD = first.distance(*getWorldRect());
		const auto d = length / numSegments;
		const auto yOffset = getHeight() / 2.0;
		const auto h = getHeight();
		auto lastRect = Rect();

		for (const auto rect : getChildrenOfType<MT::Rectangle>())
		{
			if (tD > d)
			{
				rect->setSizeAndPosition(lastRect.x - d, yOffset, d, h);
				tD -= d;
				lastRect = rect->getRect();
			}
			else if (tD > 0)
			{
				rect->setSizeAndPosition(lastRect.x - tD, yOffset, tD, h);
				break;
			}
		}
	}

	Color Trace::getSegmentColor(unsigned int pos)
	{
		Color c = getColor();
		return c.lerp(endColor, pos / numSegments);
	}
}
