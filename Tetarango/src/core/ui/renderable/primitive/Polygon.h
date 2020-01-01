#pragma once

#include "ui/renderable/primitive/Primitive.h"
#include "util/Vec.h"

namespace AW
{
	class Polygon : public Primitive
	{
		std::vector<AWVec2<double>> points, normalPoints;

		void updateSize();
		void updateNormalizedPoints();

	public:
		Polygon();

		void setPoint(AWVec2<double> p);
		void setPoint(double x, double y);
		void setPoints(const std::vector<AWVec2<double>>& points);

		const std::vector<AWVec2<double>>& getPoints();
		const std::vector<AWVec2<double>>& getNormalizedPoints();
	};
}
