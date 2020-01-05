#pragma once

#include "ui/renderable/primitive/Primitive.h"
#include "engine/module/display/VertexBufferProxy.h"
#include "util/Vec.h"

namespace AW
{
	class Polygon : public Primitive
	{
		std::vector<AWVec2<double>> screenPoints;

		bool filled = true;

		void updateSize();

	public:
		Polygon();

		std::unique_ptr<VertexBufferProxy> vertexBuffer;

		void setPoint(AWVec2<double> p);
		void setPoint(double x, double y);
		void setPoints(const std::vector<AWVec2<double>>& points);

		void centerBalancePoints();

		const std::vector<AWVec2<double>>& getScreenPoints();

		void setFilled(bool flag);
		bool getFilled();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		std::vector<AWVec2<double>> getRenderPoints();
	};
}
