#include "Polygon.h"

namespace AW
{

	Polygon::Polygon()
	{
		renderMode = RenderMode::Polygon;
		registerGameObject<Polygon>(__FUNCTION__);
	}

	void Polygon::updateSize()
	{
		if (points.size() < 2) return;

		double minX = -10000, maxX = 100000, minY = -100000, maxY = 100000;
		for (const auto& p : points)
		{
			if (p.x > minX) minX = p.x;
			if (p.x < maxX) maxX = p.x;
			if (p.y > minY) minY = p.y;
			if (p.y < maxY) maxY = p.y;
		}

		setSize(std::abs(maxX - minX), std::abs(maxY - minY));
		updateNormalizedPoints();
	}

	void Polygon::updateNormalizedPoints()
	{
		normalPoints.clear();
		double w = getWidth(), h = getHeight();
		for (const auto p : points)
		{
			normalPoints.push_back(AWVec2<double>((p.x / w) * 2.0, (p.y / h) * 2.0));
		}
	}

	void Polygon::setPoint(AWVec2<double> p)
	{
		points.push_back(p);
		updateSize();
	}

	void Polygon::setPoint(double x, double y)
	{
		points.push_back(AWVec2<double>(x, y));
		updateSize();
	}

	void Polygon::setPoints(const std::vector<AWVec2<double>>& points)
	{
		for (const auto& p : points)
		{
			this->points.push_back(p);
		}
		updateSize();
	}

	const std::vector<AWVec2<double>>& Polygon::getPoints()
	{
		return points;
	}

	const std::vector<AWVec2<double>>& Polygon::getNormalizedPoints()
	{
		return normalPoints;
	}
}
