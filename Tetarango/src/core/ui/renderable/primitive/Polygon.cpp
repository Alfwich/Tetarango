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
		GORegister(Polygon);
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

	void Polygon::centerBalancePoints()
	{
		double minX = -sizeLimit, maxX = sizeLimit, minY = -sizeLimit, maxY = sizeLimit;
		for (const auto& p : screenPoints)
		{
			if (p.x > minX) minX = p.x;
			if (p.x < maxX) maxX = p.x;
			if (p.y > minY) minY = p.y;
			if (p.y < maxY) maxY = p.y;
		}

		const auto xOffset = std::abs(maxX / 2.0) - std::abs(minX / 2.0);
		const auto yOffset = std::abs(maxY / 2.0) - std::abs(minY / 2.0);

		if (std::abs(xOffset) > 1.0 || std::abs(yOffset) > 1.0)
		{
			for (auto& p : screenPoints)
			{
				p.x += xOffset;
				p.y += yOffset;
			}
			updateSize();
		}
	}

	const std::vector<AWVec2<double>>& Polygon::getScreenPoints()
	{
		return screenPoints;
	}

	void Polygon::setFilled(bool flag)
	{
		filled = flag;
	}

	bool Polygon::getFilled()
	{
		return filled;
	}

	std::shared_ptr<SerializationClient> Polygon::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__polygon__", hint);

		filled = client->serializeBool("poly-filled", filled);

		switch (hint)
		{
		case AW::SerializationHint::SERIALIZE:
			for (unsigned int i = 0; i < screenPoints.size(); ++i)
			{
				const auto iStr = std::to_string(i);
				const auto screenPoint = screenPoints[i];
				client->setDouble("pt_x_" + iStr, screenPoint.x);
				client->setDouble("pt_y_" + iStr, screenPoint.y);
			}
			client->setInt("numPts", (int)screenPoints.size());
			break;

		case AW::SerializationHint::HYDRATE:
			screenPoints.clear();
			for (unsigned int i = 0, limit = client->getInt("numPts", 0); i < limit; ++i)
			{
				const auto iStr = std::to_string(i);
				const auto pt = AWVec2<double>(client->getDouble("pt_x_" + iStr), client->getDouble("pt_y_" + iStr));
				screenPoints.push_back(pt);
			}
			updateSize();
			break;

		case AW::SerializationHint::UNSPECIFIED:
		default:
			break;
		}

		return Primitive::doSerialize(hint);
	}

	std::vector<AWVec2<double>> Polygon::getRenderPoints()
	{
		auto result = std::vector<AWVec2<double>>();

		auto w = getWidth(), h = getHeight();
		for (const auto p : screenPoints)
		{
			result.push_back(AWVec2<double>(((p.x / w) * 2.0), ((p.y / h) * 2.0)));
		}

		return result;
	}
}
