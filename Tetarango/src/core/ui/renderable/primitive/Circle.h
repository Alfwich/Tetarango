#pragma once

#include "Primitive.h"

namespace AW
{
	class Circle : public Primitive
	{
		double radius = 0.0;
	public:
		Circle();

		void onBindShaders() override;

		void setEdgeFadeDistance(double distance);
		double getEdgeFadeDistance();

	};
}
