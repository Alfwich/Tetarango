#include "Circle.h"

namespace AW
{
	Circle::Circle()
	{
		GORegister(Circle);
	}

	void Circle::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-color", "f-circle" });
		fragmentShader->setFloatIUParam("fCircleEdge", getEdgeFadeDistance());
	}

	void Circle::setEdgeFadeDistance(double distance)
	{
		serializationClient->setDouble("circle-e-f-d", distance);

		if (fragmentShader != nullptr)
		{
			fragmentShader->setFloatIUParam("fCircleEdge", distance);
		}
	}

	double Circle::getEdgeFadeDistance()
	{
		return serializationClient->getDouble("circle-e-f-d", 0.0);
	}
}
