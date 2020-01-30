#include "Polygon.h"

namespace
{
	const auto sizeLimit = FLT_MAX;
	const auto repeatAmountParamName = "poly-r-a";
}

namespace AW
{

	Polygon::Polygon()
	{
		renderMode = RenderMode::Polygon;
		renderTextureMode = RenderTextureMode::LinearWrapping;
		GORegister(Polygon);
	}

	void Polygon::updateSize()
	{
		if (screenPoints.size() < 2) return;

		float minX = -sizeLimit, maxX = sizeLimit, minY = -sizeLimit, maxY = sizeLimit;
		for (const auto& p : screenPoints)
		{
			if (p.x > minX) minX = p.x;
			if (p.x < maxX) maxX = p.x;
			if (p.y > minY) minY = p.y;
			if (p.y < maxY) maxY = p.y;
		}

		setScreenSize(std::abs(maxX - minX), std::abs(maxY - minY));
		vertexBuffer = nullptr;
	}

	void Polygon::insertScreenPoint(float x, float y)
	{
		screenPoints.push_back(AWVec2<float>(x, y));
	}

	void Polygon::insertWorldPoint(float x, float y)
	{
		screenPoints.push_back(AWVec2<float>(Renderable::worldToScreenPosition(x), Renderable::worldToScreenPosition(y)));
	}

	void Polygon::addWorldPoint(AWVec2<float> p)
	{
		insertWorldPoint(p.x, p.y);
		updateSize();
	}

	void Polygon::addWorldPoint(float x, float y)
	{
		insertWorldPoint(x, y);
		updateSize();
	}

	void Polygon::addWorldPoints(const std::vector<AWVec2<float>>& points)
	{
		for (const auto& p : points)
		{
			insertWorldPoint(p.x, p.y);
		}

		updateSize();
	}

	void Polygon::addScreenPoint(AWVec2<float> p)
	{
		insertScreenPoint(p.x, p.y);
		updateSize();
	}

	void Polygon::addScreenPoint(float x, float y)
	{
		insertScreenPoint(x, y);
		updateSize();
	}

	void Polygon::addScreenPoints(const std::vector<AWVec2<float>>& points)
	{
		for (const auto& p : points)
		{
			insertScreenPoint(p.x, p.y);
		}

		updateSize();
	}

	void Polygon::centerBalancePoints()
	{
		float minX = -sizeLimit, maxX = sizeLimit, minY = -sizeLimit, maxY = sizeLimit;
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

	void Polygon::shiftScreenPoints(double xOffset, double yOffset)
	{
		for (auto& pt : screenPoints)
		{
			pt.x += xOffset;
			pt.y += yOffset;
		}

		updateSize();
	}

	void Polygon::shiftWorldPoints(double xOffset, double yOffset)
	{
		for (auto& pt : screenPoints)
		{
			pt.x += Renderable::worldToScreenPosition(xOffset);
			pt.y += Renderable::worldToScreenPosition(yOffset);
		}

		updateSize();
	}

	const std::vector<AWVec2<float>>& Polygon::getScreenPoints()
	{
		return screenPoints;
	}

	void Polygon::setScreenPoint(unsigned int id, float x, float y)
	{
		if (id > 0 && id < screenPoints.size())
		{
			screenPoints[id] = AWVec2<float>(x, y);
			vertexBuffer = nullptr;
		}
	}

	std::vector<AWVec2<float>> Polygon::getWorldPoints()
	{
		auto result = std::vector<AWVec2<float>>();

		for (const auto pt : screenPoints)
		{
			result.push_back(Renderable::screenToWorld(pt));
		}

		return result;
	}

	void Polygon::setFilled(bool flag)
	{
		filled = flag;
	}

	bool Polygon::getFilled()
	{
		return filled;
	}

	void Polygon::onBindShaders()
	{
		if (fragmentShader == nullptr)
		{
			fragmentShader = modules->shader->getShader({ "f-repeat", "element" });
			fragmentShader->setFloatIUParam("fRepeat", getRepeatAmount());
		}
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
				const auto pt = AWVec2<float>((float)client->getDouble("pt_x_" + iStr), (float)client->getDouble("pt_y_" + iStr));
				screenPoints.push_back(pt);
			}
			updateSize();
			break;

		case AW::SerializationHint::UNSPECIFIED:
		default:
			break;
		}

		return Element::doSerialize(hint);
	}

	void Polygon::setRepeatAmount(double amount)
	{
		serializationClient->setDouble(repeatAmountParamName, amount);
	}

	double Polygon::getRepeatAmount()
	{
		return serializationClient->getDouble(repeatAmountParamName, 1.0);
	}

	std::vector<AWVec2<float>> Polygon::getRenderPoints()
	{
		auto result = std::vector<AWVec2<float>>();

		auto w = getScreenWidth(), h = getScreenHeight();
		for (const auto p : screenPoints)
		{
			result.push_back(AWVec2<float>(((p.x / w) * 2.0), ((p.y / h) * 2.0)));
		}

		return result;
	}
}
