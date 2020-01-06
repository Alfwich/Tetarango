#pragma once

#include "ui/renderable/primitive/Primitive.h"
#include "engine/module/display/VertexBufferProxy.h"

namespace AW
{
	class Polygon : public Primitive
	{
		std::vector<AWVec2<float>> screenPoints;

		bool filled = true;

		void updateSize();

		void insertScreenPoint(float x, float y);
		void insertWorldPoint(float x, float y);

	public:
		Polygon();

		std::unique_ptr<VertexBufferProxy> vertexBuffer;

		void addScreenPoint(AWVec2<float> p);
		void addScreenPoint(float x, float y);
		void addScreenPoints(const std::vector<AWVec2<float>>& points);

		void addWorldPoint(AWVec2<float> p);
		void addWorldPoint(float x, float y);
		void addWorldPoints(const std::vector<AWVec2<float>>& points);

		void centerBalancePoints();

		const std::vector<AWVec2<float>>& getScreenPoints();
		std::vector<AWVec2<float>> getWorldPoints();

		void setFilled(bool flag);
		bool getFilled();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);

		std::vector<AWVec2<float>> getRenderPoints();
	};
}
