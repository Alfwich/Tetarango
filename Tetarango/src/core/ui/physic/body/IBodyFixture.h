#pragma once

#include "Body.h"

namespace AW
{
	enum class BodyType
	{
		Box,
		Circle,
		Polygon,
		Line,
		Chain
	};

	class IBodyFixture
	{
	public:
		b2Fixture* fixtureReference = nullptr;
		virtual const bool isSensor() const { return false; };
		virtual const BodyType getBodyType() const = 0;
		virtual const AWVec2<float> getBodyWorldPosition() { return AWVec2<float>(); };
		virtual const AWVec2<float> getBodyWorldSize() { return AWVec2<float>(); };
		virtual const std::vector<AWVec2<float>> getBodyWorldPoints() { return std::vector<AWVec2<float>>(); };
	};
}
