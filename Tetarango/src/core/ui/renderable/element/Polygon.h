#pragma once

#include "ui/renderable/element/Element.h"
#include "engine/module/display/VertexBufferProxy.h"

namespace AW
{
	class Polygon : public Element
	{
		std::vector<AWVec2<float>> screenPoints;

		bool filled = true;

		void updateSize();

		void insertScreenPoint(float x, float y);
		void insertWorldPoint(float x, float y);

	public:
		std::unique_ptr<VertexBufferProxy> vertexBuffer;

		Polygon();

		virtual void onBindShaders() override;
		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint) override;

		void addScreenPoint(AWVec2<float> p);
		void addScreenPoint(float x, float y);
		void addScreenPoints(const std::vector<AWVec2<float>>& points);

		void addWorldPoint(AWVec2<float> p);
		void addWorldPoint(float x, float y);
		void addWorldPoints(const std::vector<AWVec2<float>>& points);

		void centerBalancePoints();
		void shiftScreenPoints(double xOffset, double yOffset);
		void shiftWorldPoints(double xOffset, double yOffset);

		const std::vector<AWVec2<float>>& getScreenPoints();
		void setScreenPoint(unsigned int id, float x, float y);
		std::vector<AWVec2<float>> getWorldPoints();

		void setFilled(bool flag);
		bool getFilled();

		void setRepeatAmount(double amount);
		double getRepeatAmount();

		std::vector<AWVec2<float>> getRenderPoints();
	};
}
