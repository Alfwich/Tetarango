#include "Polygon.h"

namespace
{
	const auto sizeLimit = DBL_MAX;
}

namespace AW
{

	Polygon::Polygon()
	{
		renderMode = RenderMode::Polygon;
		registerGameObject<Polygon>(__FUNCTION__);
	}

	void Polygon::updateSize()
	{
		if (screenPoints.size() < 2) return;

		double minX = -sizeLimit, maxX = sizeLimit, minY = -sizeLimit, maxY = sizeLimit;
		for (const auto& p : screenPoints)
		{
			if (p.x > minX) minX = p.x;
			if (p.x < maxX) maxX = p.x;
			if (p.y > minY) minY = p.y;
			if (p.y < maxY) maxY = p.y;
		}

		setSize(std::abs(maxX - minX), std::abs(maxY - minY));
		vertexBuffer = nullptr;
	}

	void Polygon::setPoint(AWVec2<double> p)
	{
		screenPoints.push_back(p);
		updateSize();
	}

	void Polygon::setPoint(double x, double y)
	{
		screenPoints.push_back(AWVec2<double>(x, y));
		updateSize();
	}

	void Polygon::setPoints(const std::vector<AWVec2<double>>& points)
	{
		for (const auto& p : points)
		{
			this->screenPoints.push_back(p);
		}
		updateSize();
	}

	const std::vector<AWVec2<double>>& Polygon::getScreenPoints()
	{
		return screenPoints;
	}

	std::vector<AWVec2<double>> Polygon::getRenderPoints()
	{
		auto result = std::vector<AWVec2<double>>();

		auto w = getWidth(), h = getHeight();
		for (const auto p : screenPoints)
		{
			result.push_back(AWVec2<double>(((p.x / w) * 2.0) - 1.0, ((p.y / h) * 2.0) - 1.0));
		}

		return result;
	}
}
