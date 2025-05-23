#include "Trace.h"

#include "ui/renderable/primitive/Rectangle.h"

namespace AW
{
	Trace::Trace()
	{
		GORegister(Trace);
	}

	void Trace::onInitialAttach()
	{
		for (int i = 0; i < numSegments; ++i)
		{
			add(std::make_shared<AW::Rectangle>());
		}

		auto baseColor = getColor();
		int segmentPos = 0;
		for (const auto rect : getChildrenOfType<AW::Rectangle>())
		{
			rect->setColor(baseColor->lerp(endColor, segmentPos++ / numSegments));
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

		auto wr = Rect(getWorldRect());
		auto tD = first.distance(&wr);
		const auto d = length / numSegments;
		const auto yOffset = getScreenHeight() / 2.0;
		const auto h = getScreenHeight();
		auto lastRect = Rect();

		for (const auto rect : getChildrenOfType<AW::Rectangle>())
		{
			if (tD > d)
			{
				rect->setScreenPositionSize(lastRect.x - d, yOffset, d, h);
				tD -= d;
				lastRect = rect->getRect();
			}
			else if (tD > 0)
			{
				rect->setScreenPositionSize(lastRect.x - tD, yOffset, tD, h);
				break;
			}
		}
	}

	Color Trace::getSegmentColor(unsigned int pos)
	{
		const auto c = getColor();
		return c->lerp(endColor, pos / numSegments);
	}
}
